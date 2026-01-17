#ifndef FLIERENEMY_H
#define FLIERENEMY_H

#include <SFML/Graphics.hpp>

#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/FlappingMovement.h"
#include "entities/enemy/behaviours/movement/SlidePulseMovement.h"
#include "entities/enemy/behaviours/patrol/AirPatrol.h"
#include "entities/enemy/behaviours/patrol/XYPatrol.h"
#include "entities/enemy/behaviours/attack/DivingAttack.h"

class FlierEnemy : public Enemy
{
public:
    FlierEnemy(const sf::Vector2f &pos, float leftX, float rightX)
    {
        // Shape
        shapeColour = sf::Color::Red;

        sf::Vector2f size(16.f, 16.f);
        shape = new sf::RectangleShape(size);
        // shape->rotate(45.f); // Make rectangle a diamond!

        // sf::FloatRect b = shape->getLocalBounds();
        // shape->setOrigin(b.width * 0.5f, b.height * 0.5f);

        shape->setFillColor(shapeColour);
        shape->setPosition(pos);

        ignoreGravity = true;

        // Behaviours
        patrol = new AirPatrol({leftX, pos.y}, {rightX, pos.y});
        movement = new FlappingMovement();
        attack = new DivingAttack();

        // Speeds
        patrolSpeed = 65.f;     // quick flapping
        chaseSpeed = 120.f;     // faster flapping
        attackingSpeed = 200.f; // diving strike

        // Combat
        verticalAggroTolerance = size.y * 4.f;
        aggroRadius = 250.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 1.0f;
        attackRadius = 60.f;
        attackCooldown = 1.f;

        telegraphDuration = 0.25f;
        flashInterval = 0.08f;

        attackDamage = 15;
    }
};

#endif // FLIERENEMY_H