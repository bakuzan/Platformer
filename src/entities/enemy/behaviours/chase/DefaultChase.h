#ifndef DEFAULTCHASE_H
#define DEFAULTCHASE_H

#include "ChaseBehaviour.h"

class Enemy;

class DefaultChase : public ChaseBehaviour
{
public:
    void chase(Enemy &e, float dt, sf::Vector2f playerPosition) override
    {
        (void)dt;

        sf::Vector2f dir = playerPosition - e.getPosition();
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.f)
        {
            dir /= len;
        }
        else
        {
            dir = {0.f, 0.f};
        }

        e.setVelocity(dir);
    }
};

#endif // DEFAULTCHASE_H