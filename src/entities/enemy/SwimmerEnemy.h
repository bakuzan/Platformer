#ifndef SWIMMERENEMY_H
#define SWIMMERENEMY_H

#include <SFML/Graphics.hpp>

#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/SlidePulseMovement.h"
#include "entities/enemy/behaviours/patrol/XYPatrol.h"
#include "entities/enemy/behaviours/chase/DefaultChase.h"
#include "entities/enemy/behaviours/attack/ChargeAttack.h"
#include "entities/enemy/behaviours/attackTrigger/GroundAttackTrigger.h"

class SwimmerEnemy : public Enemy
{
public:
    SwimmerEnemy(const sf::Vector2f &pos,
                 float leftX, float rightX,
                 float topY, float bottomY)
    {
        (void)topY;
        (void)bottomY;

        sf::Vector2f size(20.f, 20.f);
        setCollider(size, pos);

        // Shape
        shapeColour = sf::Color::Red;

        visualShape = new sf::CircleShape(size.x / 2.f);
        visualShape->setFillColor(shapeColour);
        visualShape->setPosition(pos);

        // Settings
        ignoreGravity = true;

        // Behaviours
        patrol = new XYPatrol(leftX, rightX); // TODO Make AreaPatrol(lx,rx,ty,by)
        movement = new SlidePulseMovement();  // TODO SwimMovement()
        chase = new DefaultChase();
        attack = new ChargeAttack(); // TODO OmniDirectionalAttack()
        attackTrigger = new GroundAttackTrigger(72.f);

        // Speeds
        patrolSpeed = 70.f;     // default movement
        chaseSpeed = 100.f;     // aggro'd
        attackingSpeed = 150.f; // charge

        // Combat
        verticalAggroTolerance = size.y * 5.0f;
        aggroRadius = 180.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 1.0f;
        attackCooldown = 0.67f;

        telegraphDuration = 0.33f;
        flashInterval = 0.08f;

        attackDamage = 20;
    }
};

#endif // SWIMMERENEMY_H