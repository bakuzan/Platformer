#ifndef SWIMMERENEMY_H
#define SWIMMERENEMY_H

#include <SFML/Graphics.hpp>

#include "constants/MovementMedium.h"
#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/SwimMovement.h"
#include "entities/enemy/behaviours/patrol/AreaPatrol.h"
#include "entities/enemy/behaviours/chase/DefaultChase.h"
#include "entities/enemy/behaviours/attack/OmniDirectionalChargeAttack.h"
#include "entities/enemy/behaviours/attackTrigger/GroundAttackTrigger.h"

class SwimmerEnemy : public Enemy
{
public:
    SwimmerEnemy(const sf::Vector2f &pos,
                 float leftX, float rightX,
                 float topY, float bottomY)
    {
        sf::Vector2f size(20.f, 20.f);
        setCollider(size, pos);

        // Shape
        shapeColour = sf::Color::Red;

        visualShape = new sf::CircleShape(size.x / 2.f);
        visualShape->setFillColor(shapeColour);
        visualShape->setPosition(pos);

        // Core
        medium = MovementMedium::WATER;

        // Behaviours
        patrol = new AreaPatrol(leftX, rightX, topY, bottomY);
        movement = new SwimMovement();
        chase = new DefaultChase();
        attack = new OmniDirectionalChargeAttack();
        attackTrigger = new GroundAttackTrigger(72.f);

        // Speeds
        patrolSpeed = 70.f;     // default movement
        chaseSpeed = 125.f;     // aggro'd
        attackingSpeed = 275.f; // charge

        // Combat
        verticalAggroTolerance = size.y * 5.0f;
        aggroRadius = 180.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 1.0f;
        attackCooldown = 0.67f;

        telegraphDuration = 0.24f;
        flashInterval = 0.08f;

        attackDamage = 20;
    }
};

#endif // SWIMMERENEMY_H