#ifndef CHASEBEHAVIOUR_H
#define CHASEBEHAVIOUR_H

#include <SFML/Graphics.hpp>

class Enemy;

class ChaseBehaviour
{
public:
    virtual ~ChaseBehaviour() = default;

    virtual void chase(Enemy &e, float dt, sf::Vector2f playerPosition) = 0;
};

#endif // CHASEBEHAVIOUR_H