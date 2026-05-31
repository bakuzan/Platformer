#ifndef SWIM_MOVEMENT_H
#define SWIM_MOVEMENT_H

#include <SFML/Graphics.hpp>
#include <cmath>

#include "MovementBehaviour.h"
#include "entities/Enemy.h"

class SwimMovement : public MovementBehaviour
{
private:
    float time = 0.f;
    float waveFrequency;
    float waveAmplitude;

public:
    SwimMovement(float freq, float amp)
        : waveFrequency(freq), waveAmplitude(amp)
    {
    }

    void move(Enemy &e,
              float dt,
              float speed) override
    {
        time += dt;

        sf::Vector2f dir = e.getVelocity();
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (len < 0.001f)
        {
            return;
        }

        dir /= len;

        // Apply a gentle perpendicular wiggle to the forward movement
        sf::Vector2f perp(-dir.y, dir.x);

        float wiggle = std::sin(time * waveFrequency) * waveAmplitude;
        sf::Vector2f finalVel = (dir * speed) + (perp * wiggle * speed * 0.5f);

        e.setVelocity(finalVel);
    }
};

#endif // SWIM_MOVEMENT_H