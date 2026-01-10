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
        shapeColour = sf::Color::Red;

        sf::Vector2f size = {24.f, 16.f};
        shape = new sf::RectangleShape(size);
        shape->setFillColor(shapeColour);
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
        verticalAggroTolerance = size.y;
        aggroRadius = 180.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 2.0f;
        attackRadius = 72.f;
        attackCooldown = 0.67f;

        telegraphDuration = 0.33f;
        flashInterval = 0.08f;

        attackDamage = 20;
    }
};

#endif // SLUGENEMY_H