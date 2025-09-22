/// @file circle.hpp
/// @brief

#pragma once

#include "simulator.hpp"
#include "entity.hpp"

#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>

#include <SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>

class Circle : public Entity {
public:
    b2Vec2 position;
    b2Body *body;
    b2CircleShape shape;
    b2BodyType type;
    SDL_Color color;

    Circle(Simulator &sim,
           b2Vec2 _position,
           float _radius,
           b2BodyType _type,
           SDL_Color _color)
        : position(_position),
          body(),
          shape(),
          type(_type),
          color(_color)
    {
        // Adjust w.r.t. the coordinate system of SDL.
        position.y *= -1;

        // Create the body definition.
        b2BodyDef body_def;
        // Set the type.
        body_def.type = type;
        // Set the starting position x and y cartesian
        body_def.position.Set(position.x, position.y);

        // Create the actual body.
        body = sim.world.CreateBody(&body_def);

        //shape.m_p.Set(position.x, position.y);
        shape.m_radius = _radius;

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
        position.x = ((sim.scaled_width / 2.0f) + position.x) * sim.met2pix;
        position.y = ((sim.scaled_height / 2.0f) + position.y) * sim.met2pix;
    }

    ~Circle() override = default;

    inline void draw(Simulator &sim) override
    {
        if (type == b2_dynamicBody) {
            // Get the updated position.
            position = body->GetPosition();
            // Update the entity position.
            position.x = ((sim.scaled_width / 2.0f) + position.x) * sim.met2pix;
            position.y = ((sim.scaled_height / 2.0f) + position.y) * sim.met2pix;
        }
        std::cout << position.x << ", " << position.y << "\n";
        // Draw the circle.
        filledCircleRGBA(
            sim.renderer,
            position.x, position.y,
            shape.m_radius * sim.met2pix,
            color.r, color.g, color.b, color.a);
    }

private:
    SDL_Texture *texture;
};