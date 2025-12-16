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
        patrolSpeed = 40.f;     // slow crawl
        chaseSpeed = 55.f;      // slightly faster when aggro'd
        attackingSpeed = 120.f; // jump arc horizontal speed

        // Combat
        verticalAggroTolerance = 32.f;
        aggroRadius = 180.f;
        attackRadius = 60.f;
        attackCooldown = 1.2f;
        telegraphDuration = 0.25f;
    }
};

#endif // SLUGENEMY_H