/// @file box.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-01-31
///
/// @copyright Copyright (c) 2022
///

#pragma once

#include "simulator.hpp"
#include "entity.hpp"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <SDL2/SDL.h>

class Box : public Entity {
public:
    b2Vec2 position;
    float width;
    float height;
    float angle;
    b2Body *body;
    b2PolygonShape shape;
    b2BodyType type;
    SDL_Rect box;
    SDL_Color color;

    Box(Simulator &sim,
        b2Vec2 _position,
        float _width,
        float _height,
        float _angle,
        b2BodyType _type,
        SDL_Color _color)
        : position(_position),
          width(_width),
          height(_height),
          angle(_angle),
          body(),
          shape(),
          type(_type),
          box(),
          color(_color)
    {
        // Adjust w.r.t. the coordinate system of SDL.
        position.y *= -1;

        b2BodyDef body_def;
        // Set the type.
        body_def.type = type;
        // Set the starting position x and y cartesian
        body_def.position.Set(position.x, position.y);
        // Set the starting angle.
        body_def.angle = angle;

        b2Vec2 vel;
        vel.Set(0, 0.2f);

        // Create the actual body.
        body = sim.world.CreateBody(&body_def);
        body->SetLinearVelocity(vel);

        shape.SetAsBox(width / 2.0f, height / 2.0f);

        // Create the static or dynamic body.
        b2FixtureDef fixture_def;
        // Set the shape.
        fixture_def.shape = &shape;
        if (type == b2_staticBody) {
            // Nothing to do.
        } else if (type == b2_dynamicBody) {
            fixture_def.density     = 1;
            fixture_def.friction    = 0.3f;
            fixture_def.restitution = 0.5f;
        }
        body->CreateFixture(&fixture_def);

        // Set the initial entity position.
        box.x = ((sim.scaled_width / 2.0f) + position.x) * sim.met2pix - box.w / 2;
        box.y = ((sim.scaled_height / 2.0f) + position.y) * sim.met2pix - box.h / 2;
        box.w = width * sim.met2pix;
        box.h = height * sim.met2pix;

        // Create the texture for the box.
        texture = SDL_CreateTexture(
            sim.renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            box.w, box.h);
        SDL_SetRenderTarget(sim.renderer, texture);
        SDL_SetRenderDrawColor(sim.renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(sim.renderer);
        SDL_SetRenderTarget(sim.renderer, nullptr);
    }

    ~Box() override
    {
        SDL_DestroyTexture(texture);
    }

    inline void draw(Simulator &sim) override
    {
        if (type == b2_dynamicBody) {
            // Get the updated position.
            position = body->GetPosition();
            // Update the entity position.
            box.x = ((sim.scaled_width / 2.0f) + position.x) * sim.met2pix - box.w / 2;
            box.y = ((sim.scaled_height / 2.0f) + position.y) * sim.met2pix - box.h / 2;
        }
        SDL_RenderCopyEx(
            sim.renderer,
            texture,
            nullptr,
            &box,
            body->GetAngle() * RAD2DEG,
            nullptr,
            SDL_FLIP_NONE);
    }

private:
    SDL_Texture *texture;
};