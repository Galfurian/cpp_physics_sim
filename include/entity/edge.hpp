/// @file edge.hpp
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

#include <box2d/b2_edge_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>

#include <SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>

class Edge : public Entity {
public:
    b2Vec2 start;
    b2Vec2 end;
    b2Body *body;
    b2EdgeShape shape;
    b2BodyType type;
    SDL_Color color;

    Edge(Simulator &sim, b2Vec2 _start, b2Vec2 _end, b2BodyType _type, SDL_Color _color)
        : start(_start),
          end(_end),
          body(),
          shape(),
          type(_type),
          color(_color)
    {
        // Adjust w.r.t. the coordinate system of SDL.
        start.y *= -1, end.y *= -1;

        // Initialize the body constructor.
        b2BodyDef body_def;
        // Set the type.
        body_def.type = type;
        // Set the starting position x and y cartesian
        body_def.position.Set(0.f, 0.f);
        // Set the starting angle.
        body_def.angle = 0;

        // Create the actual body.
        body = sim.world.CreateBody(&body_def);
        // Set the shape coordinates.
        shape.SetTwoSided(start, end);

        // Create the static or dynamic body.
        b2FixtureDef fixture_def;
        // Set the shape.
        fixture_def.shape = &shape;
        if (type == b2_staticBody) {
            // Nothing to do.
        } else if (type == b2_dynamicBody) {
            fixture_def.density     = 0.0f;
            fixture_def.friction    = 0.0f;
            fixture_def.restitution = 0.0f;
        }
        body->CreateFixture(&fixture_def);
        // Set the initial entity position.
        start.x = ((sim.scaled_width / 2.0f) + shape.m_vertex1.x) * sim.met2pix;
        start.y = ((sim.scaled_height / 2.0f) + shape.m_vertex1.y) * sim.met2pix;
        end.x   = ((sim.scaled_width / 2.0f) + shape.m_vertex2.x) * sim.met2pix;
        end.y   = ((sim.scaled_height / 2.0f) + shape.m_vertex2.y) * sim.met2pix;
    }

    ~Edge() override = default;

    inline void draw(Simulator &sim) override
    {
        if (type == b2_dynamicBody) {
            // Get the updated position.
            b2Vec2 position = body->GetPosition();
            // Update the start and end of the line.
            start.x = ((sim.scaled_width / 2.0f) + shape.m_vertex1.x + position.x) * sim.met2pix;
            start.y = ((sim.scaled_height / 2.0f) + shape.m_vertex1.y + position.y) * sim.met2pix;
            end.x   = ((sim.scaled_width / 2.0f) + shape.m_vertex2.x + position.x) * sim.met2pix;
            end.y   = ((sim.scaled_height / 2.0f) + shape.m_vertex2.y + position.y) * sim.met2pix;
        }
        // Set the line color.
        SDL_SetRenderDrawColor(sim.renderer, color.r, color.g, color.b, color.a);
        // Draw the line.
        SDL_RenderDrawLine(sim.renderer, start.x, start.y, end.x, end.y);
    }
};