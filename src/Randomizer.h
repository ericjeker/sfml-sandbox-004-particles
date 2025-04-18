// Copyright (c) 2025 Eric Jeker. All rights reserved.

#pragma once
#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <random>
#include <vector>

enum class DistributionType
{
    Uniform,
    Gaussian,
    Perlin
};

/**
 * AI Generated Randomizer that creates different distribution of random values:
 *
 * - Uniform
 * - Gaussian
 * - Perlin (TODO: doesn't seem to work properly)
 *
 *
 * ```
 * // Generate uniform random values (default)
 * float uniformValue = Randomizer::RandomFloat(0.0f, 1.0f);
 *
 * // Switch to Gaussian distribution
 * Randomizer::SetDistributionType(DistributionType::Gaussian);
 * float gaussianValue = Randomizer::RandomFloat(0.0f, 1.0f); // Will cluster around 0.5
 *
 * // Switch to Perlin noise
 * Randomizer::SetDistributionType(DistributionType::Perlin);
 * Randomizer::ResetNoiseIndex(); // Reset the noise index before a sequence
 * float perlinValue1 = Randomizer::RandomFloat(0.0f, 1.0f);
 * float perlinValue2 = Randomizer::RandomFloat(0.0f, 1.0f); // Will be smoothly related to perlinValue1
 *
 * // For 2D Perlin noise directly
 * float noise2D = Randomizer::PerlinNoise2D(x, y);
 * ```
 */
class Randomizer
{
public:
    // Set the distribution type to use for the next random operations
    static void SetDistributionType(const DistributionType type) { currentDistribution = type; }

    // Get the current distribution type
    static DistributionType GetDistributionType() { return currentDistribution; }

    // Random unsigned char
    static unsigned char RandomUnsigned(const unsigned char min, const unsigned char max)
    {
        switch (currentDistribution)
        {
            case DistributionType::Gaussian:
                return static_cast<unsigned char>(std::clamp(
                    static_cast<int>(GaussianRandomInt(min, max)),
                    static_cast<int>(min),
                    static_cast<int>(max)
                ));

            case DistributionType::Perlin:
                // Map Perlin noise to the range [min, max]
                return static_cast<unsigned char>(min + PerlinNoise1D(noiseIndex++) * (max - min));

            case DistributionType::Uniform:
            default:
                std::uniform_int_distribution<int> dis(min, max);
                return static_cast<unsigned char>(dis(gen));
        }
    }

    // Random float
    static float RandomFloat(const float min, const float max)
    {
        switch (currentDistribution)
        {
            case DistributionType::Gaussian:
                return GaussianRandomFloat(min, max);

            case DistributionType::Perlin:
                // Map Perlin noise to the range [min, max]
                return min + PerlinNoise1D(noiseIndex++) * (max - min);

            case DistributionType::Uniform:
            default:
                std::uniform_real_distribution<float> distribution(min, max);
                return distribution(gen);
        }
    }

    // Random vector with x and y components
    static sf::Vector2f RandomVector(const float minX, const float maxX, const float minY, const float maxY)
    {
        return {RandomFloat(minX, maxX), RandomFloat(minY, maxY)};
    }

    // Random directional vector
    static sf::Vector2f RandomDirectionalVector(const sf::Vector2f &direction, const float angle)
    {
        // Special case for zero vectors - create a completely random direction
        if (direction.x == 0 && direction.y == 0) {
            // For a zero vector with any angle, create a random direction vector
            const float randomAngle = RandomFloat(0, 2 * M_PI);
            return sf::Vector2f(std::cos(randomAngle), std::sin(randomAngle));
        }

        const float baseAngle = std::atan2(direction.y, direction.x);
        const float randomAngle = baseAngle + RandomFloat(-angle / 2, angle / 2);
        const float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        return sf::Vector2f(magnitude * std::cos(randomAngle), magnitude * std::sin(randomAngle));
    }

    // Perlin noise 2D
    static float PerlinNoise2D(float x, float y)
    {
        // Initialize the permutation table if not initialized
        if (!perlinInitialized)
        {
            InitializePerlin();
        }

        // Get fractional part of x and y
        const int X = static_cast<int>(std::floor(x)) & 255;
        const int Y = static_cast<int>(std::floor(y)) & 255;
        x -= std::floor(x);
        y -= std::floor(y);

        // Compute fade curves
        const float u = Fade(x);
        const float v = Fade(y);

        // Hash coordinates of the 4 square corners
        const int A = p[X] + Y;
        const int AA = p[A];
        const int AB = p[A + 1];
        const int B = p[X + 1] + Y;
        const int BA = p[B];
        const int BB = p[B + 1];

        // Add blended results from 4 corners of the square
        return Lerp(v, Lerp(u, Grad(p[AA], x, y, 0),
                                 Grad(p[BA], x - 1, y, 0)),
                        Lerp(u, Grad(p[AB], x, y - 1, 0),
                                 Grad(p[BB], x - 1, y - 1, 0)));
    }

    // Reset the Perlin noise index
    static void ResetNoiseIndex() { noiseIndex = 0; }

private:
    static std::random_device rd;
    static std::mt19937 gen;
    static DistributionType currentDistribution;
    static unsigned int noiseIndex;
    static std::array<int, 512> p; // Permutation table for Perlin noise
    static bool perlinInitialized;

    // Gaussian distribution helpers
    static float NormalRandomFloat(const float mean, const float stddev)
    {
        std::normal_distribution<float> distribution(mean, stddev);
        return distribution(gen);
    }

    static float GaussianRandomFloat(const float min, const float max)
    {
        const float mean = (min + max) / 2.0f;
        const float stddev = (max - min) / 6.0f; // 99.7% of values will fall within [min, max]
        return std::clamp(NormalRandomFloat(mean, stddev), min, max);
    }

    static int GaussianRandomInt(const int min, const int max)
    {
        const float mean = (min + max) / 2.0f;
        const float stddev = (max - min) / 6.0f;
        return static_cast<int>(std::round(std::clamp(GaussianRandomFloat(mean, stddev),
                                            static_cast<float>(min),
                                            static_cast<float>(max))));
    }

    // Perlin noise helpers
    static void InitializePerlin()
    {
        // Initialize a permutation table with values 0-255
        std::vector<int> perm(256);
        for (int i = 0; i < 256; i++)
        {
            perm[i] = i;
        }

        // Shuffle permutation table
        std::shuffle(perm.begin(), perm.end(), gen);

        // Copy to a double-sized permutation table
        for (int i = 0; i < 256; i++)
        {
            p[i] = p[i + 256] = perm[i];
        }

        perlinInitialized = true;
    }

    static float PerlinNoise1D(const float x)
    {
        return PerlinNoise2D(x, 0.0f);
    }

    static float Fade(const float t)
    {
        // Fade function: 6t^5 - 15t^4 + 10t^3
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static float Lerp(const float t, const float a, const float b)
    {
        return a + t * (b - a);
    }

    static float Grad(const int hash, const float x, const float y, const float z)
    {
        // Convert the lower 4 bits of hash into 12 gradient directions
        const int h = hash & 15;
        const float u = h < 8 ? x : y;
        const float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};

// Static member initialization
inline std::random_device Randomizer::rd;
inline std::mt19937 Randomizer::gen{rd()};
inline DistributionType Randomizer::currentDistribution = DistributionType::Uniform;
inline unsigned int Randomizer::noiseIndex = 0;
inline std::array<int, 512> Randomizer::p{};
inline bool Randomizer::perlinInitialized = false;

#endif