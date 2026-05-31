#ifndef OMNIDIRECTIONALATTACK_H
#define OMNIDIRECTIONALATTACK_H

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "AttackBehaviour.h"
#include "entities/Enemy.h"
#include "constants/Constants.h"

class OmniDirectionalChargeAttack : public AttackBehaviour
{
private:
    float chargeDuration;
    float skidDuration;

    sf::Vector2f startPos;
    sf::Vector2f dir;
    sf::Vector2f velocity;

public:
    OmniDirectionalChargeAttack(float charge, float skid)
        : chargeDuration(charge), skidDuration(skid)
    {
    }

    void attack(Enemy &e,
                float dt,
                const sf::Vector2f &playerPos,
                float speed) override
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

            velocity = sf::Vector2f(0.f, 0.f);
        }

        timer += dt;
        float total = chargeDuration + skidDuration;

        // If the fish launches out of the water mid-charge, make it go back in
        if (!e.isSwimming())
        {
            velocity.y += Constants::GRAVITY * dt;
            velocity.x *= 0.98f; // Apply slight air resistance
        }
        else
        {
            // PHASE 1: ACCELERATION
            if (timer <= chargeDuration)
            {
                float t = timer / chargeDuration;
                velocity = dir * (speed * t);
            }
            // PHASE 2: DECELERATION
            else
            {
                float skidTime = timer - chargeDuration;
                float t = std::min(skidTime / skidDuration, 1.f);
                velocity *= (1.f - t);
            }
        }

        e.setVelocity(velocity);

        if (timer >= total)
        {
            e.setVelocity({0.f, 0.f});
            attacking = false;
            timer = 0.f;
        }
    }
};

#endif // OMNIDIRECTIONALATTACK_H