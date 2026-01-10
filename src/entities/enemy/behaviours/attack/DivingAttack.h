#ifndef DIVINGATTACK_H
#define DIVINGATTACK_H

#include <SFML/Graphics.hpp>

#include "AttackBehaviour.h"

class DivingAttack : public AttackBehavior
{
private:
    sf::Vector2f diveDir;

    float diveDuration = 0.6f; // total dive time
    float timer = 0.f;
    bool committed = false;

public:
    void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float attackingSpeed) override
    {
        sf::FloatRect b = e.getBounds();
        sf::Vector2f enemyCenter(b.left + b.width * 0.5f,
                                 b.top + b.height * 0.5f);

        if (!committed)
        {
            sf::Vector2f toTarget = playerPos - enemyCenter;
            float len = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

            if (len != 0.f)
            {
                diveDir = toTarget / len; // normalized
            }

            committed = true;
            timer = 0.f;
        }

        timer += dt;

        // Dive!
        float half = diveDuration * 0.5f;
        float speedFactor = 0.f;

        if (timer < half)
        {
            // accelerate
            float t = timer / half;
            speedFactor = t; // 0 → 1
        }
        else if (timer < diveDuration)
        {
            // decelerate
            float t = (timer - half) / half;
            speedFactor = 1.f - t; // 1 → 0
        }
        else
        {
            // dive finished
            committed = false;
            return;
        }

        float currentSpeed = attackingSpeed * speedFactor;
        sf::Vector2f vel = diveDir * currentSpeed * dt;
        e.move(vel);
    }
};

#endif // DIVINGATTACK_H
