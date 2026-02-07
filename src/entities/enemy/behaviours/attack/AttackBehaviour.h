#ifndef ATTACKBEHAVIOUR_H
#define ATTACKBEHAVIOUR_H

#include <SFML/Graphics.hpp>

class Enemy;

class AttackBehaviour
{
protected:
    bool attacking = false;
    float timer = 0.f;

public:
    virtual ~AttackBehaviour() = default;

    virtual void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float attackSpeed) = 0;

    void abortAttack()
    {
        attacking = false;
        timer = 0.f;
    }

    bool isFinished() const
    {
        return !attacking;
    }
};

#endif // ATTACKBEHAVIOUR_H