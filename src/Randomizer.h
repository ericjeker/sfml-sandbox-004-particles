// Copyright (c) 2025 Eric Jeker. All rights reserved.

#pragma once
#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <SFML/System/Vector2.hpp>
#include <random>

class Randomizer
{
public:
    static unsigned char RandomUnsigned(const unsigned char min, const unsigned char max)
    {
        std::uniform_int_distribution<unsigned char> dis(min, max);
        return dis(gen);
    }

    static float RandomFloat(const float min, const float max)
    {
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    static sf::Vector2f RandomVector(const float minX, const float maxX, const float minY, const float maxY)
    {
        return {RandomFloat(minX, maxX), RandomFloat(minY, maxY)};
    }

    static sf::Vector2f RandomDirectionalVector(const sf::Vector2f &direction, const float angle)
    {
        const float baseAngle = std::atan2(direction.y, direction.x);
        const float randomAngle = baseAngle + RandomFloat(-angle / 2, angle / 2);
        const float magnitude = direction.length();

        return sf::Vector2f(magnitude * std::cos(randomAngle), magnitude * std::sin(randomAngle));
    };

private:
    static std::random_device rd;
    static std::mt19937 gen;
};

#endif
