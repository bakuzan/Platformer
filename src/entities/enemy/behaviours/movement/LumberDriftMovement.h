#ifndef LUMBERDRIFTMOVEMENT_H
#define LUMBERDRIFTMOVEMENT_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "MovementBehaviour.h"

class Enemy;

class LumberDriftMovement : public MovementBehaviour
{
private:
    float time = 0.f;
    float frequency;
    float amplitude;
    float baseMultiplier;

public:
    LumberDriftMovement(float freq, float amp, float baseMult)
        : frequency(freq), amplitude(amp), baseMultiplier(baseMult) {}

    void move(Enemy &e, float dt, float speed) override
    {
        time += dt;

        // Dynamically calculate the drift wave based on profile params
        float drift = (std::sin(time * frequency) + 1.f) * amplitude + baseMultiplier;
        float driftSpeed = speed * drift;

        sf::Vector2f v = e.getVelocity();
        v.x *= driftSpeed;

        e.setVelocity(v);
    }
};

#endif // LUMBERDRIFTMOVEMENT_H