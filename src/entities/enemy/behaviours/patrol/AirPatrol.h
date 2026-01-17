#ifndef AIRPATROL_H
#define AIRPATROL_H

#include <SFML/Graphics.hpp>

#include "PatrolBehaviour.h"

class Enemy;

class AirPatrol : public PatrolBehavior
{
private:
    sf::Vector2f pointA;
    sf::Vector2f pointB;
    bool goingToB = true;

public:
    AirPatrol(sf::Vector2f a, sf::Vector2f b)
        : pointA(a), pointB(b) {}

    void patrol(Enemy &e, float dt) override
    {
        (void)dt;

        sf::Vector2f pos = e.getPosition();
        sf::Vector2f target = goingToB ? pointB : pointA;

        sf::Vector2f toTarget = target - pos;
        float lenSq = toTarget.x * toTarget.x + toTarget.y * toTarget.y;

        // Larger threshold because flapping adds vertical oscillation
        const float switchDistSq = 100.f; // 10 units

        if (lenSq < switchDistSq)
        {
            goingToB = !goingToB;
            return;
        }

        float len = std::sqrt(lenSq);
        sf::Vector2f dir = toTarget / len;

        e.setVelocity(dir);
    }

    float getDistFromPatrol(const sf::Vector2f &enemyPos) const override
    {
        sf::Vector2f center = {(pointA.x + pointB.x) * 0.5f,
                               (pointA.y + pointB.y) * 0.5f};

        // Euclidean distance from enemy to patrol center
        sf::Vector2f d = enemyPos - center;
        return std::sqrt(d.x * d.x + d.y * d.y);
    }
};

#endif // AIRPATROL_H