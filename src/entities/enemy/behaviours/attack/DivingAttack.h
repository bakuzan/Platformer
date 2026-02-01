#ifndef DIVINGATTACK_H
#define DIVINGATTACK_H

#include <SFML/Graphics.hpp>
#include "AttackBehaviour.h"

class DivingAttack : public AttackBehaviour
{
private:
    float diveDuration = 0.66f;
    float timer = 0.f;

    sf::Vector2f startPos;
    sf::Vector2f diveDir;

public:
    void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float attackingSpeed) override
    {
        // First frame: lock direction and start position
        if (timer == 0.f)
        {
            attacking = true;
            startPos = e.getPosition();

            sf::Vector2f toTarget = playerPos - startPos;
            float len = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

            if (len != 0.f)
            {
                diveDir = toTarget / len;
            }
            else
            {
                diveDir = {0.f, 1.f}; // fallback straight down
            }
        }

        timer += dt;

        float t = timer / diveDuration;
        if (t > 1.f)
        {
            t = 1.f;
        }

        // Ease-in, ease-out speed curve
        float speedFactor = (t < 0.5f)
                                ? (t * 2.f)        // 0 → 1
                                : (2.f - t * 2.f); // 1 → 0

        float currentSpeed = attackingSpeed * speedFactor;

        // Reverse engineer required velocity
        sf::Vector2f desiredPos = startPos + diveDir * (currentSpeed * diveDuration * t);
        sf::Vector2f currentPos = e.getPosition();
        sf::Vector2f neededVel = (desiredPos - currentPos) / dt;

        e.setVelocity(neededVel);

        // End of attack
        if (t >= 1.f)
        {
            attacking = false;
            timer = 0.f;

            sf::Vector2f settle = (startPos - e.getPosition()) * 5.f; // tiny pull
            e.setVelocity(settle);
        }
    }
};

#endif // DIVINGATTACK_H
