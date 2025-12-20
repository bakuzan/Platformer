#ifndef SLUGENEMY_H
#define SLUGENEMY_H

#include <SFML/Graphics.hpp>

#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/SlidePulseMovement.h"
#include "entities/enemy/behaviours/patrol/XYPatrol.h"
#include "entities/enemy/behaviours/attack/JumpAttack.h"

class SlugEnemy : public Enemy
{
public:
    SlugEnemy(const sf::Vector2f &pos, float leftX, float rightX)
    {
        // Shape
        shape = new sf::RectangleShape({24.f, 16.f});
        shape->setFillColor(sf::Color::Red);
        shape->setPosition(pos);

        // Behaviours
        patrol = new XYPatrol(leftX, rightX);
        movement = new SlidePulseMovement();
        attack = new JumpAttack();

        // Speeds
        patrolSpeed = 75.f;     // slow crawl
        chaseSpeed = 90.f;      // slightly faster when aggro'd
        attackingSpeed = 100.f; // jump arc horizontal speed

        // Combat
        verticalAggroTolerance = 20.f;
        aggroRadius = 180.f;
        attackRadius = 72.f;
        attackCooldown = 0.67f;
        telegraphDuration = 0.33f;
    }
};

#endif // SLUGENEMY_H