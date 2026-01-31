#ifndef SWIM_MOVEMENT_H
#define SWIM_MOVEMENT_H

#include <SFML/Graphics.hpp>
#include <cmath>

#include "MovementBehaviour.h"

class Enemy;

class SwimMovement : public MovementBehaviour
{
private:
    float time = 0.f;

    float waveFrequency = 3.0f; // how fast the fish wiggles
    float waveAmplitude = 0.4f; // how strong the wiggle is (affects sideways drift)

public:
    void move(Enemy &e, float dt, float speed) override
    {
        time += dt;

        // Base forward movement from velocity
        sf::Vector2f dir = e.getVelocity();

        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len < 0.001f)
        {
            return;
        }

        dir /= len;

        // Perpendicular vector to direction
        sf::Vector2f perp(-dir.y, dir.x);

        // Sine-wave offset
        float wiggle = std::sin(time * waveFrequency) * waveAmplitude;
        sf::Vector2f finalVel = (dir * speed) + (perp * wiggle * speed * 0.5f);

        e.setVelocity(finalVel);
    }
};

#endif // SWIM_MOVEMENT_H
