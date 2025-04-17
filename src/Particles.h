// Copyright (c) 2025 Eric Jeker. All rights reserved.

#pragma once
#ifndef PARTICLES_H
#define PARTICLES_H

#include <SFML/System/Vector2.hpp>
#include <vector>

struct Particles
{
    std::vector<sf::Vector2f> positions;
    std::vector<float> lifeTimes;
    std::vector<float> timeRemainder;
};

#endif
