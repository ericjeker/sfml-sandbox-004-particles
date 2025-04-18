// Copyright (c) 2025 Eric Jeker. All rights reserved.

#include <SFML/Graphics/RectangleShape.hpp>
#include <random>

#include "ParticleEmitter.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const unsigned screenWidth, const unsigned screenHeight)
    : _screenWidth(screenWidth)
    , _screenHeight(screenHeight)
{
}

void ParticleSystem::Initialize(const unsigned nbrParticles)
{
    // Shapes vector
    _vertices = sf::VertexArray(sf::PrimitiveType::Points);

    // Particle data
    _particles.positions.reserve(nbrParticles);
    _particles.velocities.reserve(nbrParticles);
    _particles.scales.reserve(nbrParticles);
    _particles.colors.reserve(nbrParticles);
    _particles.lifeTimes.reserve(nbrParticles);
    _particles.timeRemainder.reserve(nbrParticles);
}

// ----------------------------------------------------------------------------
// Emitter Management
//

void ParticleSystem::SpawnEmitter(std::unique_ptr<ParticleEmitter> emitter) { _emitters.push_back(std::move(emitter)); }

// ----------------------------------------------------------------------------
// Particle Management
//

void ParticleSystem::SpawnParticle(const sf::Vector2f position, const sf::Vector2f velocity, const sf::Color color,
                                   const float lifeTime)
{
    _particles.positions.emplace_back(position);
    _particles.velocities.emplace_back(velocity);
    _particles.scales.emplace_back(sf::Vector2f{1.f, 1.f});
    _particles.colors.emplace_back(color);
    _particles.lifeTimes.emplace_back(lifeTime);
    _particles.timeRemainder.emplace_back(lifeTime);
}

bool ParticleSystem::HasExpired(const size_t i) const { return _particles.timeRemainder[i] <= 0.f; }
bool ParticleSystem::IsOutOfBounds(const size_t index) const
{
    return _particles.positions[index].x > _screenWidth || _particles.positions[index].y > _screenHeight ||
           _particles.positions[index].x < 0 || _particles.positions[index].y < 0;
}

void ParticleSystem::KillParticle(const size_t index)
{
    // Get the last particle and replace the one we want to kill
    _particles.positions[index] = _particles.positions.back();
    _particles.velocities[index] = _particles.velocities.back();
    _particles.scales[index] = _particles.scales.back();
    _particles.colors[index] = _particles.colors.back();
    _particles.lifeTimes[index] = _particles.lifeTimes.back();
    _particles.timeRemainder[index] = _particles.timeRemainder.back();

    _particles.positions.pop_back();
    _particles.velocities.pop_back();
    _particles.scales.pop_back();
    _particles.colors.pop_back();
    _particles.lifeTimes.pop_back();
    _particles.timeRemainder.pop_back();

    _vertices[index] = sf::Vertex(); // Reset the vertex to avoid flickering
}

// ----------------------------------------------------------------------------
// Update, Render
//

void ParticleSystem::Update(const sf::Time &time)
{
    const auto elapsed = time.asSeconds();
    for (int i = _particles.positions.size() - 1; i >= 0; --i)
    {
        if (HasExpired(i) || IsOutOfBounds(i))
        {
            // Kill the particle as it reached the end of its life
            KillParticle(i);
            continue;
        }

        // Particle Physics Calculations
        _particles.positions[i] += _particles.velocities[i] * elapsed;
        _particles.timeRemainder[i] -= elapsed;
    }

    for (int i = _emitters.size() - 1; i >= 0; --i)
    {
        _emitters[i]->Update(time);
        if (!_emitters[i]->IsActive())
        {
            _emitters.erase(_emitters.begin() + i);
            --i;
        }
    }

    // TODO: This is adding another loop over all the particles
    UpdateVertices();
}

void ParticleSystem::UpdateVertices()
{
    // Resize the vertex array to match the number of particles
    _vertices.resize(_particles.positions.size());

    // Update each vertex with its particle data
    for (size_t i = 0; i < _particles.positions.size(); ++i)
    {
        _vertices[i].position = _particles.positions[i];
        _vertices[i].color = _particles.colors[i];
    }
}


void ParticleSystem::Render(sf::RenderWindow &target) const { target.draw(_vertices); }

unsigned long long ParticleSystem::GetNumberOfParticles() const
{
    return _particles.positions.size();
}
