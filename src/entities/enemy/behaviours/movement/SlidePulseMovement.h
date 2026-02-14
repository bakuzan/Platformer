#ifndef SLIDEPULSEMOVEMENT_H
#define SLIDEPULSEMOVEMENT_H

#include <SFML/Graphics.hpp>

#include "MovementBehaviour.h"

class Enemy;

class SlidePulseMovement : public MovementBehaviour
{
private:
    float time = 0.f;

public:
    void move(Enemy &e, float dt, float speed) override
    {
        time += dt;

        // Pulse between 0 and 1
        float pulse = (std::sin(time * 6.f) + 1.f) * 0.5f;

        // Movement bursts
        float burstSpeed = speed * pulse;

        sf::Vector2f v = e.getVelocity();
        v.x *= burstSpeed;

        e.setVelocity(v);
    }
};

#endif // SLIDEPULSEMOVEMENT_H