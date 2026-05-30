#ifndef FLYINGCHASE_H
#define FLYINGCHASE_H

#include "ChaseBehaviour.h"
#include <cmath>

class FlyingChase : public ChaseBehaviour
{
private:
    float preferredY;
    float verticalDrift = 1.0f; // Let it track cleanly up/down

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
            dir.x = (dx > 0.f) ? 1.f : -1.f;
        }

        // Vertical Tracking: Maintain a dynamic altitude above the player
        const float targetAltitudeAbovePlayer = 70.f;
        float targetY = playerPos.y - targetAltitudeAbovePlayer;

        // Give it a small dead-zone buffer (e.g., 5 pixels) to prevent jittering
        if (pos.y < targetY - 5.f)
        {
            dir.y = verticalDrift; // Move down closer to player's altitude
        }
        else if (pos.y > targetY + 5.f)
        {
            dir.y = -verticalDrift; // Fly back up to get space
        }

        // Normalize
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.001f)
        {
            dir.x /= len;
            dir.y /= len;
        }

        e.setVelocity(dir);
    }
};

#endif // FLYINGCHASE_H