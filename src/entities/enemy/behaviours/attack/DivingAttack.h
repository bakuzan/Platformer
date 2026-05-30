#ifndef DIVINGATTACK_H
#define DIVINGATTACK_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include "AttackBehaviour.h"

class DivingAttack : public AttackBehaviour
{
private:
    float duration = 1.0f; // Now dynamically calculated per dive
    sf::Vector2f p0;
    sf::Vector2f p1;
    sf::Vector2f p2;

public:
    DivingAttack() {}

    void attack(Enemy &e, float dt,
                const sf::Vector2f &playerPos,
                float diveSpeed) override
    {
        if (timer == 0.f)
        {
            attacking = true;
            p0 = e.getPosition();

            // 1. Establish End point (P2)
            p2.x = p0.x + 2.f * (playerPos.x - p0.x);

            // Instead of p0.y, target a cruising altitude above the player's attacked position
            const float swoopExitAltitude = 70.f;
            p2.y = playerPos.y - swoopExitAltitude;

            // 2. Establish Control point (P1) using the intersection formula
            p1.x = 2.f * playerPos.x - 0.5f * p0.x - 0.5f * p2.x;
            p1.y = 2.f * playerPos.y - 0.5f * p0.y - 0.5f * p2.y;

            // 3. Estimate Arc Length (unchanged)
            float chordLength = std::sqrt((p2.x - p0.x) * (p2.x - p0.x) + (p2.y - p0.y) * (p2.y - p0.y));
            float lenP0ToP1 = std::sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
            float lenP1ToP2 = std::sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
            float polyLength = lenP0ToP1 + lenP1ToP2;

            float estimatedArcLength = (chordLength + polyLength) / 2.f;

            // 4. Dynamic Duration
            duration = (diveSpeed > 0.f) ? (estimatedArcLength / diveSpeed) : 1.f;
        }

        timer += dt;
        float t = std::min(timer / duration, 1.f);

        // Calculate position along the curve
        float u = 1.f - t;
        sf::Vector2f desiredPos = (u * u * p0) + (2.f * u * t * p1) + (t * t * p2);
        sf::Vector2f currentPos = e.getPosition();
        sf::Vector2f neededVel = (desiredPos - currentPos) / dt;

        e.setVelocity(neededVel);

        if (t >= 1.f)
        {
            attacking = false;
            timer = 0.f;
        }
    }
};

#endif // DIVINGATTACK_H