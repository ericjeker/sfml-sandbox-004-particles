// Copyright (c) 2025 Eric Jeker. All rights reserved.

#include "ParticleEmitter.h"

#include <iostream>

#include "ParticleSystem.h"
#include "Randomizer.h"

#include <bits/ostream.tcc>
#include <random>

ParticleEmitter::ParticleEmitter(ParticleSystem &system, const sf::Vector2f &position)
    : _system(system)
{
    // Randomizer::SetDistributionType(DistributionType::Gaussian);
    // Randomizer::ResetNoiseIndex();
    _emitterProps.position = position;
}

void ParticleEmitter::Update(const sf::Time &time)
{
    _timeElapsed += time.asSeconds();
    _active = _timeElapsed < _emitterProps.duration;
    if (_active)
    {
        // Emit the Particles in the System
        Emit(time);
    }
}

void ParticleEmitter::Emit(const sf::Time &time)
{
    // Use the emissionRate to determine if particles should be emitted
    const float emissionInterval = 1.0f / _emitterProps.emissionRate;
    _emissionAccumulator += time.asSeconds();

    if (_emissionAccumulator >= emissionInterval)
    {
        // Calculate how many emissions should occur
        const int emissionCount = static_cast<int>(_emissionAccumulator / emissionInterval);
        _emissionAccumulator -= emissionCount * emissionInterval;

        // Emit particles based on the calculated emission count
        for (int e = 0; e < emissionCount; ++e)
        {
            for (int i = 0; i < _emitterProps.particlesPerEmission; ++i)
            {
                const auto direction =
                        Randomizer::RandomDirectionalVector(_emitterProps.direction, _emitterProps.angle).normalized() *
                        Randomizer::RandomFloat(_particleProps.minVelocity, _particleProps.maxVelocity);

                const float lifeTime = Randomizer::RandomFloat(_particleProps.minLifetime, _particleProps.maxLifetime);

                // Generate particles
                _system.SpawnParticle(_emitterProps.position, direction, _particleProps.color, lifeTime);
            }
        }
    }
}

// ----------------------------------------------------------------------------
// Getters
//

bool ParticleEmitter::IsActive() const { return _active; }

// ----------------------------------------------------------------------------
// Setters
//

void ParticleEmitter::SetVelocity(const float &min, const float &max)
{
    _particleProps.minVelocity = min;
    _particleProps.maxVelocity = max;
}
void ParticleEmitter::SetLifetime(const float &min, const float &max)
{
    _particleProps.minLifetime = min;
    _particleProps.maxLifetime = max;
}
void ParticleEmitter::SetPosition(const sf::Vector2f &position) { _emitterProps.position = position; }
void ParticleEmitter::SetDirection(const sf::Vector2f &direction) { _emitterProps.direction = direction; }
void ParticleEmitter::SetAngle(const float &angle) { _emitterProps.angle = angle; }
void ParticleEmitter::SetDuration(const float &duration)
{
    _emitterProps.duration = duration;
    _timeElapsed = 0.f;
}
void ParticleEmitter::SetColor(const sf::Color &color) { _particleProps.color = color; }
void ParticleEmitter::SetActive(const bool &active)
{
    _active = active;
    _timeElapsed = 0.f;
}
void ParticleEmitter::SetParticlesPerEmission(const unsigned int &count) { _emitterProps.particlesPerEmission = count; }
void ParticleEmitter::SetEmissionRate(const float &emissionsPerSecond)
{
    _emitterProps.emissionRate = emissionsPerSecond;
    _emissionAccumulator = 0.f;
}
