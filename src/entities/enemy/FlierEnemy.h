#ifndef FLIERENEMY_H
#define FLIERENEMY_H

#include <SFML/Graphics.hpp>

#include "constants/MovementMedium.h"
#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/FlappingMovement.h"
#include "entities/enemy/behaviours/patrol/AirPatrol.h"
#include "entities/enemy/behaviours/chase/FlyingChase.h"
#include "entities/enemy/behaviours/attack/DivingAttack.h"
#include "entities/enemy/behaviours/attackTrigger/FlyingAttackTrigger.h"

class FlierEnemy : public Enemy
{
public:
    FlierEnemy(const sf::Vector2f &pos, float leftX, float rightX)
    {
        // Size larger than collider to ensure that it looks like it hits
        sf::Vector2f colliderSize(16.f, 16.f);
        setCollider(colliderSize, pos);

        // Shape
        shapeColour = sf::Color::Red;

        sf::Vector2f shapeSize(20.f, 20.f);
        sf::ConvexShape *triangle = new sf::ConvexShape(3);
        triangle->setPoint(0, {shapeSize.x * 0.5f, 0.f});  // top
        triangle->setPoint(1, {0.f, shapeSize.y});         // left
        triangle->setPoint(2, {shapeSize.x, shapeSize.y}); // right
        triangle->setFillColor(shapeColour);

        visualShape = triangle;

        sf::FloatRect vb = triangle->getLocalBounds();
        visualOffset.x = (colliderSize.x - vb.width) / 2.f;
        visualOffset.y = (colliderSize.y - vb.height) / 2.f;

        // Core
        medium = MovementMedium::AIR;

        // Behaviours
        patrol = new AirPatrol({leftX, pos.y}, {rightX, pos.y});
        movement = new FlappingMovement();
        chase = new FlyingChase(pos.y);
        attack = new DivingAttack();
        attackTrigger = new FlyingAttackTrigger(80.f, colliderSize.y * 5.f);

        // Speeds
        patrolSpeed = 65.f;     // quick flapping
        chaseSpeed = 120.f;     // faster flapping
        attackingSpeed = 280.f; // diving strike

        // Combat
        verticalAggroTolerance = colliderSize.y * 5.f;
        aggroRadius = 250.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 1.0f;
        attackCooldown = 0.5f;

        telegraphDuration = 0.2f;
        flashInterval = 0.08f;

        attackDamage = 15;
    }
};

#endif // FLIERENEMY_H