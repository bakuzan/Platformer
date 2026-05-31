#ifndef JUMPATTACK_H
#define JUMPATTACK_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

#include "constants/Constants.h"
#include "AttackBehaviour.h"

class JumpAttack : public AttackBehaviour
{
private:
    float desiredJumpHeight;
    float maxHorizontalRange;

public:
    JumpAttack(float jumpHeightInPixels, float maxRange)
        : desiredJumpHeight(jumpHeightInPixels),
          maxHorizontalRange(maxRange)
    {
    }

    void attack(Enemy &e,
                float dt,
                const sf::Vector2f &playerPos,
                float speed) override
    {
        (void)dt;
        (void)speed;

        if (!attacking)
        {
            attacking = true;

            float dx = playerPos.x - e.getPosition().x;
            float directionX = (dx > 0.f) ? 1.f : -1.f;

            if (std::abs(dx) > maxHorizontalRange)
            {
                dx = directionX * maxHorizontalRange;
            }

            // Kinematic Calculations
            float g = Constants::GRAVITY;
            float h = desiredJumpHeight;

            float flightTime = std::sqrt((8.0f * h) / g);
            float vx = dx / flightTime;
            float vy = -std::sqrt(2.0f * g * h);

            e.setVelocity({vx, vy});
        }
        else
        {
            // Fall protection: only register landing while moving downward
            bool isFalling = e.getVelocity().y >= 0.f;

            if (isFalling && e.isGrounded())
            {
                e.setVelocity({0.f, 0.f});
                attacking = false;
            }
        }
    }
};

#endif // JUMPATTACK_H