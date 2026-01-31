#ifndef AREAPATROL_H
#define AREAPATROL_H

#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

#include "PatrolBehaviour.h"

class Enemy;

class AreaPatrol : public PatrolBehaviour
{
private:
    float leftX, rightX,
        topY, bottomY;

    sf::Vector2f dir;       // current movement direction
    float stickyTime = 0.f; // how long we keep current direction
    float stickyMin = 0.8f;
    float stickyMax = 2.0f;

    std::mt19937 rng;
    std::uniform_real_distribution<float> distDir{-1.f, 1.f};
    std::uniform_real_distribution<float> distSticky;

private:
    sf::Vector2f randomDirection()
    {
        sf::Vector2f d(distDir(rng), distDir(rng));

        float len = std::sqrt(d.x * d.x + d.y * d.y);
        if (len < 0.001f)
        {
            return {1.f, 0.f}; // fallback
        }

        return d / len;
    }

public:
    AreaPatrol(float lx, float rx,
               float ty, float by)
        : leftX(lx), rightX(rx), topY(ty), bottomY(by),
          distSticky(stickyMin, stickyMax)
    {
        std::random_device rd;
        rng.seed(rd());

        dir = randomDirection();
        stickyTime = distSticky(rng);
    }

    void patrol(Enemy &e, float dt) override
    {
        sf::Vector2f pos = e.getPosition();

        // Decrease sticky timer
        stickyTime -= dt;

        // Pick a new direction if timer expired
        if (stickyTime <= 0.f)
        {
            dir = randomDirection();
            stickyTime = distSticky(rng);
        }

        // Bounce off boundaries
        if (pos.x <= leftX &&
            dir.x < 0.f)
        {
            dir.x = -dir.x;
        }

        if (pos.x >= rightX &&
            dir.x > 0.f)
        {
            dir.x = -dir.x;
        }

        if (pos.y <= topY &&
            dir.y < 0.f)
        {
            dir.y = -dir.y;
        }

        if (pos.y >= bottomY &&
            dir.y > 0.f)
        {
            dir.y = -dir.y;
        }

        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.001f)
        {
            dir /= len;
        }

        e.setVelocity(dir);
    }

    float getDistFromPatrol(const sf::Vector2f &pos) const override
    {
        float cx = (leftX + rightX) * 0.5f;
        float cy = (topY + bottomY) * 0.5f;

        float dx = pos.x - cx;
        float dy = pos.y - cy;

        return std::sqrt(dx * dx + dy * dy);
    }
};

#endif // AREAPATROL_H
