// Copyright (c) 2025 Eric Jeker. All rights reserved.

#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Particles.h"


class ParticleSystem {
public:
    ParticleSystem(const unsigned screenWidth, const unsigned screenHeight);
    ~ParticleSystem() = default;

    void Initialize(unsigned nbrParticles);
    void KillParticle(size_t size);
    void SpawnParticle();
    void Update(const sf::Time &time);
    void Render(sf::RenderWindow &window);
private:
    Particles _particles;
    std::vector<sf::RectangleShape> _shapes;

    const unsigned _screenWidth;
    const unsigned _screenHeight;
};



#endif
