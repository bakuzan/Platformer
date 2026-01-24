#ifndef FLYINGCHASE_H
#define FLYINGCHASE_H

#include "ChaseBehaviour.h"

class Enemy;

class FlyingChase : public ChaseBehaviour
{
private:
    float preferredY;            // cruising altitude
    float verticalDrift = 0.25f; // gentle correction speed

public:
    FlyingChase(float startY)
        : preferredY(startY) {}

    void chase(Enemy &e, float dt, sf::Vector2f playerPos) override
    {
        (void)dt;

        sf::Vector2f pos = e.getPosition();
        sf::Vector2f dir = {0.f, 0.f};

        // Horizontal tracking
        float dx = playerPos.x - pos.x;
        if (std::abs(dx) > 1.f)
        {
            dir.x = (dx > 0.f ? 1.f : -1.f);
        }

        // Maintain altitude
        float dy = preferredY - pos.y;
        if (std::abs(dy) > 1.f)
        {
            dir.y = (dy > 0.f ? verticalDrift : -verticalDrift);
        }

        // Guarantee non-zero direction
        if (dir.x == 0.f &&
            dir.y == 0.f)
        {
            dir.x = 0.0001f;
        }

        // Normalize
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.001f)
        {
            dir /= len;
        }

        e.setVelocity(dir);
    }
};

#endif // FLYINGCHASE_H