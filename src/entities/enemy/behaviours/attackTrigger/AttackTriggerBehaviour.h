#ifndef ATTACKTRIGGERBEHAVIOUR_H
#define ATTACKTRIGGERBEHAVIOUR_H

#include <SFML/Graphics.hpp>

class Enemy;

class AttackTriggerBehaviour
{
public:
    virtual ~AttackTriggerBehaviour() = default;

    virtual bool shouldAttack(const Enemy &e, const sf::Vector2f &playerPos) = 0;
};

#endif // ATTACKTRIGGERBEHAVIOUR_H