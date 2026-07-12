#ifndef SURFACEMOVEMENT_H
#define SURFACEMOVEMENT_H

#include <SFML/Graphics.hpp>

#include "MovementBehaviour.h"

class Enemy;

class SurfaceMovement : public MovementBehaviour
{
public:
    void move(Enemy &e, float dt, float speed) override
    {
        (void)dt;

        sf::Vector2f v = e.getVelocity();
        v.x *= speed;
        v.y *= speed;

        e.setVelocity(v);
    }
};

#endif // SURFACEMOVEMENT_H