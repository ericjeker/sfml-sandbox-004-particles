#include <SFML/Graphics.hpp>
#include <random>

#include "ParticleSystem.h"
#include "Randomizer.h"


constexpr unsigned NBR_PARTICLES = 10000;
constexpr unsigned SCREEN_WIDTH = 1920u;
constexpr unsigned SCREEN_HEIGHT = 1080u;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Particles Project");
    window.setFramerateLimit(144);

    // FPS Text
    const sf::Font font("assets/Orbitron-Regular.ttf");
    auto fpsText = sf::Text(font);
    fpsText.setCharacterSize(10);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition({20.f, 20.f});

    // FPS Text background
    sf::RectangleShape fpsTextBackground(sf::Vector2f(80.f, 30.f));
    fpsTextBackground.setFillColor(sf::Color{255, 255, 255, 25});
    fpsTextBackground.setPosition({10.f, 10.f});

    // Refresh every second
    float fpsRefresh = 1.f;

    // Particles vector
    ParticleSystem particleSystem(SCREEN_WIDTH, SCREEN_HEIGHT);
    particleSystem.Initialize(NBR_PARTICLES);

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

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
        }

        // -----------------------------------------------------------
        // Update

        particleSystem.Update(elapsed);

        if (fpsRefresh <= 0.f)
        {
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(1.f / elapsed.asSeconds())));
            fpsRefresh = 1.f;
        }
        else
        {
            fpsRefresh -= elapsed.asSeconds();
        }

        // -----------------------------------------------------------
        // Render

        window.clear();
        // Render the particles
        particleSystem.Render(window);
        // Render the FPS text
        window.draw(fpsTextBackground);
        window.draw(fpsText);
        window.display();
    }
}
