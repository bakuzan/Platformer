#include <SFML/Graphics.hpp>

#include "entities/Enemy.h"

class AttackBehavior
{
public:
    virtual ~AttackBehavior() = default;

    virtual void attack(Enemy &e, float dt, const sf::Vector2f &playerPos, float attackSpeed) = 0;
};
