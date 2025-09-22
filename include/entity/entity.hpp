/// @file entity.hpp
/// @brief

#pragma once

class Simulator;

class Entity {
public:
    virtual ~Entity() = default;

    virtual void draw(Simulator &sim) = 0;
};