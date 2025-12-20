#ifndef ATTACKBEHAVIOUR_H
#define ATTACKBEHAVIOUR_H

#include <SFML/Graphics.hpp>

class Enemy;

class AttackBehavior
{
protected:
    bool attacking = false;

public:
    virtual ~AttackBehavior() = default;

    virtual void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float attackSpeed) = 0;

    bool isFinished() const
    {
        return !attacking;
    }
};

#endif // ATTACKBEHAVIOUR_H