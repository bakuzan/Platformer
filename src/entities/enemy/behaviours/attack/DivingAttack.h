#ifndef DIVINGATTACK_H
#define DIVINGATTACK_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include "AttackBehaviour.h"

class DivingAttack : public AttackBehaviour
{
private:
    float diveDuration = 1.0f;
    float skidDuration = 0.4f;

    sf::Vector2f p0, p1, p2;
    sf::Vector2f exitVelocity;

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
            const float swoopExitAltitude = 70.f;
            p2.y = playerPos.y - swoopExitAltitude;

            // 2. Establish Control point (P1)
            p1.x = 2.f * playerPos.x - 0.5f * p0.x - 0.5f * p2.x;
            p1.y = 2.f * playerPos.y - 0.5f * p0.y - 0.5f * p2.y;

            // 3. Estimate Arc Length
            float chordLength = std::sqrt((p2.x - p0.x) * (p2.x - p0.x) + (p2.y - p0.y) * (p2.y - p0.y));
            float lenP0ToP1 = std::sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
            float lenP1ToP2 = std::sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
            float polyLength = lenP0ToP1 + lenP1ToP2;

            float estimatedArcLength = (chordLength + polyLength) / 2.f;

            // Dynamic Duration
            diveDuration = (diveSpeed > 0.f) ? (estimatedArcLength / diveSpeed) : 1.f;
        }

        timer += dt;

        // PHASE 1: The Dive
        if (timer <= diveDuration)
        {
            float t = timer / diveDuration;
            float u = 1.f - t;

            sf::Vector2f desiredPos = (u * u * p0) + (2.f * u * t * p1) + (t * t * p2);
            sf::Vector2f currentPos = e.getPosition();
            sf::Vector2f neededVel = (desiredPos - currentPos) / dt;

            e.setVelocity(neededVel);
            exitVelocity = neededVel; // Continuously track the velocity
        }
        // PHASE 2: The Smooth Skid
        else if (timer <= diveDuration + skidDuration)
        {
            float skidTimer = timer - diveDuration;
            float skidT = skidTimer / skidDuration;

            // Aim for 30% of the exit speed (a smooth glide)
            sf::Vector2f targetCoastVel = exitVelocity * 0.3f;

            // Lerp between the fast exit velocity and the slow coasting velocity
            sf::Vector2f currentVel = exitVelocity * (1.f - skidT) + targetCoastVel * skidT;
            e.setVelocity(currentVel);
        }
        // FINISHED
        else
        {
            attacking = false;
            timer = 0.f;
        }
    }
};

#endif // DIVINGATTACK_H