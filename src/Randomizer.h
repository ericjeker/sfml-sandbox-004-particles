// Copyright (c) 2025 Eric Jeker. All rights reserved.

#pragma once
#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <SFML/System/Vector2.hpp>
#include <random>

class Randomizer
{
public:
    static float RandomFloat(float min, float max)
    {
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    static sf::Vector2f RandomVector(float minX, float maxX, float minY, float maxY)
    {
        return sf::Vector2f(RandomFloat(minX, maxX), RandomFloat(minY, maxY));
    }

private:
    static std::random_device rd;
    static std::mt19937 gen;
};

#endif
