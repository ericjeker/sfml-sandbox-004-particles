// Copyright (c) 2025 Eric Jeker. All rights reserved.

#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Particles.h"

// Forward Declaration
class ParticleEmitter;
class ParticleSystem {
public:
    ParticleSystem(const unsigned screenWidth, const unsigned screenHeight);
    ~ParticleSystem() = default;

    void Initialize(unsigned nbrParticles);

    // Create and manage particles
    void SpawnParticle(sf::Vector2f position, sf::Vector2f velocity, sf::Color color, float lifeTime);
    // Calculate if a particle is out of the bounds defined by screenWidth and screenHeight
    bool IsOutOfBounds(const size_t index) const;
    void KillParticle(const size_t index);
    bool HasExpired(size_t i) const;

    // Create and manage emitters
    void SpawnEmitter(std::unique_ptr<ParticleEmitter> emitter);

    void UpdateVertices();
    // Time function update
    void Update(const sf::Time &time);
    // Render function with render target
    void Render(sf::RenderWindow &target) const;

private:
    // Boundaries, outside them the particles are killed
    const unsigned _screenWidth;
    const unsigned _screenHeight;

    // The list of Emitters currently in the system (AoS)
    std::vector<std::unique_ptr<ParticleEmitter>> _emitters;

    // Particle data (SoA)
    Particles _particles;
    // SFML vertices that can be given a position, texture, and color
    sf::VertexArray _vertices;
};



#endif
