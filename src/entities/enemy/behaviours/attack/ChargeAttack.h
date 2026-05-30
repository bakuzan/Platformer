#ifndef CHARGEATTACK_H
#define CHARGEATTACK_H

#include <SFML/Graphics.hpp>
#include <cmath>

#include "AttackBehaviour.h"

class ChargeAttack : public AttackBehaviour
{
private:
    float chargeDuration;
    float skidDuration;

    float directionX = 1.f;
    float currentSpeedX = 0.f;

public:
    ChargeAttack(float chargeDuration_, float skidDuration_)
        : chargeDuration(chargeDuration_), skidDuration(skidDuration_) {}

    void attack(Enemy &e,
                float dt,
                const sf::Vector2f &playerPos,
                float attackingSpeed) override
    {
        if (timer == 0.f)
        {
            attacking = true;

            // Determine horizontal direction once at the start of the charge
            float dx = playerPos.x - e.getPosition().x;
            directionX = (dx >= 0.f) ? 1.f : -1.f;
            currentSpeedX = 0.f;
        }

        timer += dt;
        float totalTime = chargeDuration + skidDuration;

        // -------------------------
        // PHASE 1: ACCELERATION
        // -------------------------
        if (timer <= chargeDuration)
        {
            float t = timer / chargeDuration;
            currentSpeedX = attackingSpeed * t; // Linearly ramp up to max speed
        }
        // -------------------------
        // PHASE 2: SKID / DECELERATION
        // -------------------------
        else
        {
            float skidTime = timer - chargeDuration;
            float t = skidTime / skidDuration;

            if (t > 1.f)
            {
                t = 1.f;
            }

            // Linearly reduce velocity from max down to zero
            currentSpeedX = attackingSpeed * (1.f - t);
        }

        // Apply horizontal speed, but strictly maintain current vertical velocity (gravity!)
        e.setVelocity({directionX * currentSpeedX, e.getVelocity().y});

        // End of attack
        if (timer >= totalTime)
        {
            attacking = false;
            timer = 0.f;
        }
    }
};

#endif // CHARGEATTACK_H