#ifndef SLUGENEMY_H
#define SLUGENEMY_H

#include <SFML/Graphics.hpp>

#include "constants/MovementMedium.h"
#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/SlidePulseMovement.h"
#include "entities/enemy/behaviours/patrol/XYPatrol.h"
#include "entities/enemy/behaviours/chase/DefaultChase.h"
#include "entities/enemy/behaviours/attack/JumpAttack.h"
#include "entities/enemy/behaviours/attackTrigger/GroundAttackTrigger.h"

class SlugEnemy : public Enemy
{
public:
    SlugEnemy(const sf::Vector2f &pos, float leftX, float rightX)
    {
        sf::Vector2f size(24.f, 16.f);
        setCollider(size, pos);

        // Shape
        shapeColour = sf::Color::Red;

        visualShape = new sf::RectangleShape(size);
        visualShape->setFillColor(shapeColour);
        visualShape->setPosition(pos);

        // Core
        medium = MovementMedium::LAND;

        // Behaviours
        patrol = new XYPatrol(leftX, rightX);
        movement = new SlidePulseMovement();
        chase = new DefaultChase();
        attack = new JumpAttack();
        attackTrigger = new GroundAttackTrigger(72.f);

        // Speeds
        patrolSpeed = 75.f;     // slow crawl
        chaseSpeed = 90.f;      // slightly faster when aggro'd
        attackingSpeed = 100.f; // jump arc horizontal speed

        // Combat
        verticalAggroTolerance = size.y;
        aggroRadius = 180.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 2.0f;
        attackCooldown = 0.67f;

        telegraphDuration = 0.33f;
        flashInterval = 0.08f;

        attackDamage = 20;
    }
};

#endif // SLUGENEMY_H