#ifndef JUMPATTACK_H
#define JUMPATTACK_H

#include <SFML/Graphics.hpp>

#include "AttackBehaviour.h"

class JumpAttack : public AttackBehavior
{
private:
    float duration = 0.5f; // total jump time
    float height = 80.f;   // peak height
    float timer = 0.f;
    sf::Vector2f startPos;
    sf::Vector2f targetPos;

public:
    void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float speed) override
    {
        if (timer == 0.f)
        {
            startPos = e.shape->getPosition();
            targetPos = playerPos;
        }

        timer += dt;
        float t = timer / duration;

        if (t > 1.f)
        {
            t = 1.f;
        }

        // Linear horizontal interpolation
        sf::Vector2f pos = startPos + (targetPos - startPos) * t;

        // Parabolic vertical offset
        float yOffset = -height * (4.f * t * (1.f - t));
        pos.y += yOffset;

        e.shape->setPosition(pos);

        if (t >= 1.f)
        {
            timer = 0.f; // reset for next attack
        }
    }
};

#endif // JUMPATTACK_H
