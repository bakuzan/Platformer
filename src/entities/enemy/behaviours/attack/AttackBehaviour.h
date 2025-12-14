#ifndef ATTACKBEHAVIOUR_H
#define ATTACKBEHAVIOUR_H

#include <SFML/Graphics.hpp>

class Enemy;

class AttackBehavior
{
public:
    virtual ~AttackBehavior() = default;

    virtual void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float attackSpeed) = 0;
};

#endif // ATTACKBEHAVIOUR_H