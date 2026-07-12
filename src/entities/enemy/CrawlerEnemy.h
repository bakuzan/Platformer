#ifndef CRAWLERENEMY_H
#define CRAWLERENEMY_H

#include <SFML/Graphics.hpp>

#include "constants/Constants.h"
#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/SurfaceMovement.h"
#include "entities/enemy/behaviours/patrol/SurfacePatrol.h"

class CrawlerEnemy : public Enemy
{
public:
    CrawlerEnemy(const sf::Vector2f &pos)
    {
        sf::Vector2f size(24.f, 24.f);
        setCollider(size, pos);

        shapeColour = sf::Color::Red;
        visualShape = new sf::RectangleShape(size);
        visualShape->setFillColor(shapeColour);
        visualShape->setPosition(pos);

        medium = MovementMedium::SURFACE_CRAWLER;
        int startingHealth = 40;
        maxHealth = startingHealth;
        health = startingHealth;

        patrolSpeed = 200.f;

        // Peaceful crawler - no aggro!
        aggroRadius = 0.f;
        chaseRadius = 0.f;

        patrol = new SurfacePatrol();
        movement = new SurfaceMovement();

        chase = nullptr;
        attack = nullptr;
        attackTrigger = nullptr;
    }

    void applyEnvironmentForces(float dt) override
    {
        (void)dt;

        if (auto *surfacePatrol = dynamic_cast<SurfacePatrol *>(patrol))
        {
            sf::Vector2f stickyGravDir = surfacePatrol->getGravityDir();

            // Apply a gentle "gravity" force into the wall so turning doesn't break
            float stickyForce = 45.f;

            sf::Vector2f currentVel = getVelocity();
            currentVel.x += stickyGravDir.x * stickyForce;
            currentVel.y += stickyGravDir.y * stickyForce;

            setVelocity(currentVel);
        }
    }

    void applyPhysicsResult(PhysicsResult &res) override
    {
        sf::Vector2f beforeVel = velocity;
        Enemy::applyPhysicsResult(res);

        if (auto *surfacePatrol = dynamic_cast<SurfacePatrol *>(patrol))
        {
            surfacePatrol->handlePhysics(beforeVel, res);
        }
    }
};

#endif // CRAWLERENEMY_H