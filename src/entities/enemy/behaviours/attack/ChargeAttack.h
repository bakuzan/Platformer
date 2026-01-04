#ifndef CHARGEATTACK_H
#define CHARGEATTACK_H

#include <SFML/Graphics.hpp>

#include "AttackBehaviour.h"

class ChargeAttack : public AttackBehavior
{
private:
    float chargeDuration = 0.7f; // time spent accelerating
    float skidDuration = 0.3f;   // time spent decelerating
    float timer = 0.f;

    sf::Vector2f startPos;
    sf::Vector2f dir;
    sf::Vector2f velocity;

public:
    void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float attackingSpeed) override
    {
        if (timer == 0.f)
        {
            attacking = true;
            startPos = e.getPosition();
            dir = playerPos - startPos;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (len != 0.f)
            {
                dir /= len;
            }

            velocity = sf::Vector2f(0.f, 0.f); // start from rest
        }

        timer += dt;
        float totalTime = chargeDuration + skidDuration;

        // ------------------------- // PHASE 1: ACCELERATION // -------------------------
        if (timer <= chargeDuration)
        {
            float t = timer / chargeDuration;
            float accel = attackingSpeed * t;

            // linear acceleration
            velocity = dir * accel;
        }
        // ------------------------- // PHASE 2: SKID / DECELERATION // -------------------------
        else
        {
            float skidTime = timer - chargeDuration;
            float t = skidTime / skidDuration;

            if (t > 1.f)
            {
                t = 1.f;
            }

            // linearly reduce velocity to zero
            velocity *= (1.f - t);
        }

        // Apply movement
        sf::Vector2f pos = e.getPosition() + velocity * dt;
        e.setPosition(pos);

        // End of attack
        if (timer >= totalTime)
        {
            attacking = false;
            timer = 0.f;
        }
    }
};

#endif // CHARGEATTACK_H
