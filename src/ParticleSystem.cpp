// Copyright (c) 2025 Eric Jeker. All rights reserved.

#include "ParticleSystem.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <random>

#include "Randomizer.h"

std::random_device Randomizer::rd;
std::mt19937 Randomizer::gen(Randomizer::rd());

ParticleSystem::ParticleSystem(const unsigned screenWidth, const unsigned screenHeight)
    : _screenWidth(screenWidth)
    , _screenHeight(screenHeight)
{
}

void ParticleSystem::Initialize(const unsigned nbrParticles)
{
    // Shapes vector
    _shapes.reserve(nbrParticles);

    // Particle data
    _particles.positions.reserve(nbrParticles);
    _particles.lifeTimes.reserve(nbrParticles);
    _particles.timeRemainder.reserve(nbrParticles);

    for (size_t i = 0; i < nbrParticles; ++i)
    {
        sf::RectangleShape shape(sf::Vector2f(2.f, 2.f));
        shape.setFillColor(sf::Color::White);
        _shapes.emplace_back(shape);

        SpawnParticle();
    }
}

void ParticleSystem::KillParticle(size_t size)
{
    // Get the last particle and replace the one we want to kill
    const auto position = _particles.positions.back();
    const auto lifeTime = _particles.lifeTimes.back();
    const auto timeRemainder = _particles.timeRemainder.back();

    _particles.positions[size] = position;
    _particles.lifeTimes[size] = lifeTime;
    _particles.timeRemainder[size] = timeRemainder;

    _particles.positions.erase(_particles.positions.begin() + _particles.positions.size() - 1);
    _particles.lifeTimes.erase(_particles.lifeTimes.begin() + _particles.lifeTimes.size() - 1);
    _particles.timeRemainder.erase(_particles.timeRemainder.begin() + _particles.timeRemainder.size() - 1);
}

void ParticleSystem::SpawnParticle()
{
    _particles.positions.emplace_back(Randomizer::RandomVector(0.f, _screenWidth, 0.f, _screenHeight));

    float lifeTime = Randomizer::RandomFloat(0.5f, 2.0f);
    _particles.lifeTimes.emplace_back(lifeTime);
    _particles.timeRemainder.emplace_back(lifeTime);
}

void ParticleSystem::Update(const sf::Time &time)
{
    const auto elapsed = time.asSeconds();
    for (size_t i = 0; i < _particles.positions.size(); ++i)
    {
        _particles.timeRemainder[i] -= elapsed;
        if (_particles.timeRemainder[i] <= 0.f)
        {
            KillParticle(i);
            SpawnParticle();
        }
    }
}

void ParticleSystem::Render(sf::RenderWindow &window)
{
    for (size_t i = 0; i < _particles.positions.size(); ++i)
    {
        _shapes[i].setPosition(_particles.positions[i]);
        _shapes[i].setFillColor(
                sf::Color{255, 255, 255,
                          static_cast<unsigned char>(255 * _particles.timeRemainder[i] / _particles.lifeTimes[i])});
        window.draw(_shapes[i]);
    }
}
