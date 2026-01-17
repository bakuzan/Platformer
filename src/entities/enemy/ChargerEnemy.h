#ifndef CHARGERENEMY_H
#define CHARGERENEMY_H

#include <SFML/Graphics.hpp>

#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/LumberDriftMovement.h"
#include "entities/enemy/behaviours/patrol/XYPatrol.h"
#include "entities/enemy/behaviours/attack/ChargeAttack.h"

class ChargerEnemy : public Enemy
{
public:
    ChargerEnemy(const sf::Vector2f &pos, float leftX, float rightX)
    {
        sf::Vector2f size(60.f, 60.f);
        setCollider(size, pos);

        // Shape
        shapeColour = sf::Color::Red;

        visualShape = new sf::RectangleShape(size);
        visualShape->setFillColor(shapeColour);
        visualShape->setPosition(pos);

        // Behaviours
        patrol = new XYPatrol(leftX, rightX);
        movement = new LumberDriftMovement();
        attack = new ChargeAttack();

        // Speeds
        patrolSpeed = 40.f;     // slow crawl
        chaseSpeed = 100.f;     // a lot faster when aggro'd
        attackingSpeed = 200.f; // charge horizontal speed

        // Combat
        verticalAggroTolerance = size.y / 2.f;
        aggroRadius = 224.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 3.0f;
        attackRadius = 125.f;
        attackCooldown = 1.f;

        telegraphDuration = 0.33f;
        flashInterval = 0.08f;

        attackDamage = 40;
    }
};

#endif // CHARGERENEMY_H