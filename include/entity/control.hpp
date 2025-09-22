/// @file control.hpp
/// @brief

#pragma once

#include "entity/entity.hpp"
#include "entity/circle.hpp"
#include "entity/edge.hpp"

class Control : public Entity {
public:
    Circle *circle;
    Edge *plane;

    Control(Circle *_circle, Edge *_plane)
        : circle(_circle),
          plane(_plane),
          applied_force()
    {
        // Nothing to do.
    }

    ~Control()
    {
        delete circle;
        delete plane;
    }

    inline void draw(Simulator &sim) override
    {
        auto circle_position = circle->body->GetPosition();
        // auto circle_velocity = circle->body->GetLinearVelocity();
        if (-circle_position.y < 0.2) {
            if (!applied_force) {
                // Define the force vector (Newton).
                b2Vec2 force{ -2000.f, 2000.f };
                // Apply the force to the ball.
                circle->body->ApplyForce(force, circle_position, false);
                applied_force = true;
            }
        } else {
            applied_force = false;
        }
        circle->draw(sim);
        plane->draw(sim);
    }

private:
    bool applied_force;
};