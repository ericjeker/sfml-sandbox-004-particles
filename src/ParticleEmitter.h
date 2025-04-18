// Copyright (c) 2025 Eric Jeker. All rights reserved.

#pragma once
#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <math.h>

// Forward Declaration
class ParticleSystem;
class ParticleEmitter
{
public:
    ParticleEmitter(ParticleSystem &system, const sf::Vector2f &position);
    ~ParticleEmitter() = default;

    // Time calculation to deactivate the Emitter
    void Update(const sf::Time &time);

    // Actually generate the particles and add them to the ParticleSystem
    void Emit(const sf::Time &time);

    // Getters
    bool IsActive() const;

    // Setters
    void SetPosition(const sf::Vector2f &position);
    void SetDirection(const sf::Vector2f &direction);
    void SetAngle(const float &angle);
    void SetDuration(const float &duration);
    void SetColor(const sf::Color &color);
    void SetActive(const bool &active);
    void SetParticlesPerEmission(const unsigned int &count);
    void SetEmissionRate(const float &emissionsPerSecond);
    void SetVelocity(const float &min, const float &max);
    void SetLifetime(const float &min, const float &max);

private:
    // A reference to the particle system where we'll emit particles
    ParticleSystem &_system;

    // ---------------------------------------------------------------------------------
    // Configuration
    //

    struct
    {
        // Color of the vertices
        sf::Color color = sf::Color::White;
        sf::Color startColor = sf::Color::White;
        sf::Color endColor = sf::Color::Transparent;
        float minLifetime = 1.0f;
        float maxLifetime = 3.0f;
        // Initial velocity of the particle
        float minVelocity = 1.0f;
        float maxVelocity = 2.0f;
    } _particleProps;

    struct
    {
        // Position of the Emitter and direction of the emission
        sf::Vector2f position;
        // The direction in which the particles will be emitted
        sf::Vector2f direction = sf::Vector2f(0.f, 0.f);
        // The angle of emission, in Radians
        float angle = 2 * M_PI;
        // The duration of the emission, in seconds
        float duration = 2.f;
        // Number of particle counts per emission
        unsigned int particlesPerEmission = 10;
        // How many times per second to emit particles
        float emissionRate = 10.f;
    } _emitterProps;

    // ---------------------------------------------------------------------------------
    // States
    //

    // Is the emitter active?
    bool _active = false;
    // Time Accumulator (reset to 0.0f when the duration is changed)
    float _timeElapsed = 0.0f;
    // Time since last emission (set to 0.0f when the emissionRate is changed)
    float _emissionAccumulator = 0.0f;
};


#endif
