// Copyright (c) 2025 Eric Jeker. All rights reserved.

#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Randomizer.h"

#include <random>

std::random_device Randomizer::rd;
std::mt19937 Randomizer::gen(Randomizer::rd());

ParticleEmitter::ParticleEmitter(ParticleSystem &system, const sf::Vector2f &position)
    : _system(system)
{
    _emitterProps.position = position;
}

void ParticleEmitter::Update(const sf::Time &time)
{
    _timeElapsed += time.asSeconds();
    _active = _timeElapsed < _emitterProps.duration;
    if (_active)
    {
        // Emit the Particles in the System
        Emit();
    }
}

void ParticleEmitter::Emit() const
{
    // Spawn 10 particles every frame for the lifetime of the emitter
    for (int i = 0; i < _emitterProps.particlesPerEmission; ++i)
    {
        // Generate particles
        _system.SpawnParticle(_emitterProps.position,
                              Randomizer::RandomDirectionalVector(_emitterProps.direction, _emitterProps.angle),
                              _particleProps.color, Randomizer::RandomFloat(0.5f, 2.0f));
    }
}

// ----------------------------------------------------------------------------
// Getters
//

bool ParticleEmitter::IsActive() const { return _active; }

// ----------------------------------------------------------------------------
// Setters
//

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
void ParticleEmitter::SetParticlesPerEmission(const unsigned int count) { _emitterProps.particlesPerEmission = count; }
void ParticleEmitter::SetEmissionRate(const float emissionsPerSecond)
{
    _emitterProps.emissionRate = emissionsPerSecond;
    _emissionAccumulator = 0.f;
}
