#ifndef LUMBERDRIFTMOVEMENT_H
#define LUMBERDRIFTMOVEMENT_H

#include <SFML/Graphics.hpp>
#include "MovementBehaviour.h"

class Enemy;

class LumberDriftMovement : public MovementBehaviour
{
private:
    float time = 0.f;

public:
    void move(Enemy &e, float dt, float speed) override
    {
        time += dt;

        // Slow heavy drift between 0.4 and 1.0
        float drift = (std::sin(time * 1.2f) + 1.f) * 0.075f + 0.85f;
        float driftSpeed = speed * drift;

        sf::Vector2f v = e.getVelocity();
        v.y = 0.f;

        e.setVelocity(v * driftSpeed);
    }
};

#endif // LUMBERDRIFTMOVEMENT_H
