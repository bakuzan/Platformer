#ifndef JUMPATTACK_H
#define JUMPATTACK_H

#include <SFML/Graphics.hpp>
#include <cmath>

#include "AttackBehaviour.h"

class JumpAttack : public AttackBehaviour
{
private:
    float duration = 0.5f; // total jump time
    float jumpHeight;      // peak height of the parabola
    float idealRange;      // horizontal distance to cover in duration

    sf::Vector2f startPos;
    sf::Vector2f velocity;

public:
    JumpAttack(float jumpHeight_, float idealHorizontalRange)
        : jumpHeight(jumpHeight_),
          idealRange(idealHorizontalRange)
    {
    }

    void attack(Enemy &e,
                float dt,
                const sf::Vector2f &playerPos,
                float attackingSpeed) override
    {
        (void)attackingSpeed;

        // First frame: compute trajectory
        if (timer == 0.f)
        {
            attacking = true;
            startPos = e.getPosition();

            // Determine direction (left or right)
            sf::Vector2f dir = playerPos - startPos;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (len != 0.f)
            {
                dir /= len;
            }

            // Horizontal speed chosen to land exactly at idealRange
            float horizontalSpeed = idealRange / duration;
            velocity.x = (dir.x >= 0.f ? 1.f : -1.f) * horizontalSpeed;
            velocity.y = 0.f;
        }

        // Progress jump
        timer += dt;
        float t = timer / duration;
        if (t > 1.f)
        {
            t = 1.f;
        }

        // Parabolic arc
        sf::Vector2f desiredPos = startPos;
        desiredPos.x += velocity.x * (t * duration);
        desiredPos.y = startPos.y - jumpHeight * (4.f * t * (1.f - t));

        sf::Vector2f currentPos = e.getPosition();
        sf::Vector2f neededVel = (desiredPos - currentPos) / dt;

        e.setVelocity(neededVel);

        // End of attack
        if (t >= 1.f)
        {
            attacking = false;
            timer = 0.f;
        }
    }
};

#endif // JUMPATTACK_H
