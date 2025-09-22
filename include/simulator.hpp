/// @file sdl_config.hpp
/// @brief

#pragma once

#include "entity/entity.hpp"
#include "defines.hpp"

#include <box2d/b2_world.h>
#include <SDL2/SDL.h>
#include <vector>

class Simulator {
public:
    const float met2pix;
    const unsigned width;
    const unsigned height;
    const float scaled_width;
    const float scaled_height;
    b2World world;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::vector<Entity *> entities;

    Simulator(
        unsigned _met2pix      = 80,
        unsigned _width        = 640,
        unsigned _height       = 480,
        const b2Vec2 &_gravity = b2Vec2(0.f, 9.81f))
        : met2pix(_met2pix),
          width(_width),
          height(_height),
          scaled_width(width / met2pix),
          scaled_height(height / met2pix),
          world(_gravity),
          window(),
          renderer()
    {
        // Nothing to do.
    }

    ~Simulator()
    {
        for (auto entity : entities) {
            delete entity;
        }
    }

    void draw()
    {
        // Clear the screen.
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0);
        SDL_RenderClear(renderer);
        // Draw the entities.
        for (auto entity : entities) {
            entity->draw(*this);
        }
    }

    void update()
    {
        // Update the physics simulator.
        world.Step(1.0f, 10, 10);
    }

    void render()
    {
        // Render the scene.
        SDL_RenderPresent(renderer);
    }
};
