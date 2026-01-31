#ifndef JUMPATTACK_H
#define JUMPATTACK_H

#include <SFML/Graphics.hpp>

#include "AttackBehaviour.h"

class JumpAttack : public AttackBehaviour
{
private:
    float duration = 0.5f; // total jump time
    float height = 72.f;   // peak height
    float timer = 0.f;

    sf::Vector2f startPos;
    sf::Vector2f velocity;

public:
    void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float attackingSpeed) override
    {
        // First frame: compute real velocity
        if (timer == 0.f)
        {
            attacking = true;
            startPos = e.getPosition();

            sf::Vector2f dir = playerPos - startPos;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (len != 0.f)
            {
                dir /= len;
            }

            velocity = dir * attackingSpeed;
        }

        timer += dt;
        float t = timer / duration;

        if (t > 1.f)
        {
            t = 1.f;
        }

        sf::Vector2f pos = startPos + velocity * (t * duration);

        float yOffset = -height * (4.f * t * (1.f - t));
        pos.y += yOffset;

        e.setPosition(pos);

        if (t >= 1.f)
        {
            attacking = false;
            timer = 0.f; // reset for next attack
        }
    }
};

#endif // JUMPATTACK_H
