#ifndef FLAPPINGMOVEMENT_H
#define FLAPPINGMOVEMENT_H

#include <SFML/Graphics.hpp>

#include "MovementBehaviour.h"

class Enemy;

class FlappingMovement : public MovementBehaviour
{
private:
    float time = 0.f;
    sf::Vector2f lastDir = {1.f, 0.f};

public:
    void move(Enemy &e, float dt, float speed) override
    {
        time += dt;

        sf::Vector2f v = e.getVelocity();
        float len = std::sqrt(v.x * v.x + v.y * v.y);

        if (len == 0.f)
        {
            v = lastDir;
        }

        sf::Vector2f dir = v / len; // normalise
        lastDir = dir;              // store last valid direction

        // Horizontal movement
        sf::Vector2f finalVelocity = dir * speed;

        // Vertical flapping
        float amplitude = 30.f;
        float frequency = 3.f;
        float flapVy = std::cos(time * frequency) * amplitude;
        finalVelocity.y += flapVy;

        e.move(finalVelocity * dt);
    }
};

#endif // FLAPPINGMOVEMENT_H
