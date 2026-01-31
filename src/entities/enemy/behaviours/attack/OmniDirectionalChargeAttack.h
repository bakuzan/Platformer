#ifndef OMNIDIRECTIONALATTACK_H
#define OMNIDIRECTIONALATTACK_H

#include <SFML/Graphics.hpp>

#include "AttackBehaviour.h"

class OmniDirectionalChargeAttack : public AttackBehaviour
{
private:
    float chargeDuration = 0.75f; // accelerating
    float skidDuration = 0.25f;   // decelerating
    float timer = 0.f;

    sf::Vector2f startPos;
    sf::Vector2f dir;
    sf::Vector2f velocity;

public:
    void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float speed) override
    {
        if (timer == 0.f)
        {
            attacking = true;
            startPos = e.getPosition();

            // Compute full 360° direction
            dir = playerPos - startPos;

            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (len != 0.f)
            {
                dir /= len;
            }

            velocity = sf::Vector2f(0.f, 0.f); // start from rest
        }

        timer += dt;
        float total = chargeDuration + skidDuration;

        // ------------------------- // PHASE 1: ACCELERATION // -------------------------
        if (timer <= chargeDuration)
        {
            float t = timer / chargeDuration;
            float accel = speed * t;
            velocity = dir * accel;
        }
        // ------------------------- // PHASE 2: DECELERATION // -------------------------
        else
        {
            float skidTime = timer - chargeDuration;
            float t = skidTime / skidDuration;

            if (t > 1.f)
            {
                t = 1.f;
            }

            velocity *= (1.f - t);
        }

        // Apply movement
        sf::Vector2f pos = e.getPosition() + velocity * dt;
        e.setPosition(pos);

        // End of attack
        if (timer >= total)
        {
            attacking = false;
            timer = 0.f;
        }
    }
};

#endif // OMNIDIRECTIONALATTACK_H
