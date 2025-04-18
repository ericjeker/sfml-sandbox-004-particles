// Copyright (c) 2025 Eric Jeker. All rights reserved.

#pragma once
#ifndef PARTICLES_H
#define PARTICLES_H

#include <SFML/System/Vector2.hpp>
#include <vector>

struct Particles
{
    std::vector<sf::Vector2f> positions;
    std::vector<sf::Vector2f> velocities;
    std::vector<sf::Vector2f> scales;
    std::vector<sf::Color> colors;

    std::vector<float> lifeTimes;
    std::vector<float> timeRemainder;
};

#endif
