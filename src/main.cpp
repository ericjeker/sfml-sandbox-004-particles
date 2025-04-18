#include <SFML/Graphics.hpp>
#include <random>

#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Randomizer.h"

constexpr unsigned SCREEN_WIDTH = 1920u;
constexpr unsigned SCREEN_HEIGHT = 1080u;
constexpr unsigned NBR_PARTICLES = 100000;

namespace DraculaColors
{
    constexpr sf::Color BLACK(0x28, 0x2A, 0x36);
    constexpr sf::Color WHITE(0xF8, 0xF8, 0xF2);
    constexpr sf::Color CYAN(0x8B, 0xE9, 0xFD);
    constexpr sf::Color GREEN(0x50, 0xFA, 0x7B);
    constexpr sf::Color ORANGE(0xFF, 0xB8, 0x6C);
    constexpr sf::Color PINK(0xFF, 0x79, 0xC6);
    constexpr sf::Color PURPLE(0xBD, 0x93, 0xF9);
    constexpr sf::Color RED(0xFF, 0x55, 0x55);
    constexpr sf::Color YELLOW(0xF1, 0xFA, 0x8C);

    sf::Color WithAlpha(const sf::Color& color, const unsigned char& alpha)
    {
        return sf::Color(color.r, color.g, color.b, alpha);
    }
}


int main()
{
    // ------------------------------------------------------------------------
    // Initialization
    //

    // Render target, for now it's the window
    auto window = sf::RenderWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Particles Project");
    window.setFramerateLimit(144);

    sf::RectangleShape background(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    background.setFillColor(DraculaColors::BLACK);

    // FPS Text (Debug)
    const sf::Font font("assets/Orbitron-Regular.ttf");
    auto fpsText = sf::Text(font);
    fpsText.setCharacterSize(10);
    fpsText.setFillColor(DraculaColors::WHITE);
    fpsText.setPosition({20.f, 20.f});
    auto nbrParticlesText = sf::Text(font);
    nbrParticlesText.setCharacterSize(10);
    nbrParticlesText.setFillColor(DraculaColors::WHITE);
    nbrParticlesText.setPosition({20.f, 42.f});

    // FPS Text background
    sf::RectangleShape debugTextBackground(sf::Vector2f(120.f, 62.f));
    debugTextBackground.setFillColor(DraculaColors::WithAlpha(DraculaColors::WHITE, 28));
    debugTextBackground.setPosition({10.f, 10.f});

    // Refresh the text every second so it's readable
    float fpsRefresh = 1.f;

    // Particles system initialization
    ParticleSystem particleSystem(SCREEN_WIDTH, SCREEN_HEIGHT);
    // TODO: Do I need a 2 steps initialization really? Why?
    particleSystem.Initialize(NBR_PARTICLES);

    // ------------------------------------------------------------------------
    // Game loop
    //

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time time = clock.restart();

        // --------------------------------------------------------------------
        // Handle Events & Inputs

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
                {
                    window.close();
                }
            }

            if (auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f vector2F(static_cast<float>(mousePressed->position.x),
                                      static_cast<float>(mousePressed->position.y));

                // Create ParticleEmitters and spawn them into the system
                auto smoke = std::make_unique<ParticleEmitter>(particleSystem, vector2F);
                smoke->SetColor(DraculaColors::WHITE);
                smoke->SetDuration(5.f);
                smoke->SetEmissionRate(5.f);
                smoke->SetDirection(sf::Vector2f{0.f, 0.f});
                smoke->SetVelocity(5.f, 10.f);
                smoke->SetLifetime(2.f, 5.f);
                smoke->SetParticlesPerEmission(200);
                particleSystem.SpawnEmitter(std::move(smoke));
                auto yellow = std::make_unique<ParticleEmitter>(particleSystem, vector2F);
                yellow->SetColor(DraculaColors::YELLOW);
                yellow->SetDuration(.5f);
                yellow->SetEmissionRate(5.f);
                yellow->SetDirection(sf::Vector2f{0.f, 0.f});
                yellow->SetVelocity(1.f, 50.f);
                yellow->SetLifetime(.1f, .5f);
                yellow->SetParticlesPerEmission(200);
                particleSystem.SpawnEmitter(std::move(yellow));
                auto orange = std::make_unique<ParticleEmitter>(particleSystem, vector2F);
                orange->SetColor(DraculaColors::ORANGE);
                orange->SetDuration(.5f);
                orange->SetEmissionRate(3.f);
                orange->SetDirection(sf::Vector2f{0.f, 0.f});
                orange->SetVelocity(1.f, 50.f);
                orange->SetLifetime(.1f, .5f);
                orange->SetParticlesPerEmission(200);
                particleSystem.SpawnEmitter(std::move(orange));
                auto fire = std::make_unique<ParticleEmitter>(particleSystem, vector2F);
                fire->SetColor(DraculaColors::PINK);
                fire->SetDuration(.1f);
                fire->SetEmissionRate(100.f);
                fire->SetDirection(sf::Vector2f{0.f, 0.f});
                fire->SetVelocity(1.f, 50.f);
                fire->SetLifetime(.1f, .5f);
                fire->SetParticlesPerEmission(200);
                particleSystem.SpawnEmitter(std::move(fire));
                auto blast = std::make_unique<ParticleEmitter>(particleSystem, vector2F);
                blast->SetColor(DraculaColors::CYAN);
                blast->SetDuration(1.f / 100.f);
                blast->SetEmissionRate(1000.f);
                blast->SetDirection(sf::Vector2f{0.f, 0.f});
                blast->SetVelocity(495.f, 505.f);
                blast->SetLifetime(1.f, 2.f);
                blast->SetParticlesPerEmission(3000);
                particleSystem.SpawnEmitter(std::move(blast));
            }
        }

        // -----------------------------------------------------------
        // Update

        // This is where "everything" happens
        particleSystem.Update(time);

        // Refresh the debug
        nbrParticlesText.setString("Particles: " + std::to_string(particleSystem.GetNumberOfParticles()));

        // Refresh the FPS debug text
        if (fpsRefresh <= 0.f)
        {
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(1.f / time.asSeconds())));
            fpsRefresh = 1.f;
        }
        else
        {
            fpsRefresh -= time.asSeconds();
        }

        // -----------------------------------------------------------
        // Render

        window.clear();

        window.draw(background);

        // Render the particles
        particleSystem.Render(window);

        // Render the FPS text
        window.draw(debugTextBackground);
        window.draw(fpsText);
        window.draw(nbrParticlesText);

        window.display();
    }
}
