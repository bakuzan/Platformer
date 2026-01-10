#ifndef FLIERENEMY_H
#define FLIERENEMY_H

#include <SFML/Graphics.hpp>

#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/FlappingMovement.h"
#include "entities/enemy/behaviours/patrol/XYPatrol.h"
#include "entities/enemy/behaviours/attack/DivingAttack.h"

class FlierEnemy : public Enemy
{
public:
    FlierEnemy(const sf::Vector2f &pos, float leftX, float rightX)
    {
        // Shape
        shapeColour = sf::Color::Red;

        sf::ConvexShape *tri = new sf::ConvexShape();
        tri->setPointCount(3);

        // Place the three points of triangle
        sf::Vector2f size(16.f, 16.f);
        tri->setPoint(0, {0.f, size.y});       // bottom-left
        tri->setPoint(1, {size.x, size.y});    // bottom-right
        tri->setPoint(2, {size.x / 2.f, 0.f}); // top-center

        sf::FloatRect b = tri->getLocalBounds();
        tri->setOrigin(b.width * 0.5f, b.height * 0.5f);

        tri->setFillColor(shapeColour);
        tri->setPosition(pos);

        shape = tri;

        ignoreGravity = true;

        // Behaviours
        patrol = new XYPatrol(leftX, rightX);
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