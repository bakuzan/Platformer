#ifndef SLUGENEMY_H
#define SLUGENEMY_H

#include <SFML/Graphics.hpp>
#include <cassert>

#include "constants/Constants.h"
#include "constants/MovementMedium.h"

#include "data/profiles/JumpAttackProfile.h"

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
        // ------------------------------------------------------------
        // Collider + Visual
        // ------------------------------------------------------------
        sf::Vector2f size(24.f, 16.f);
        setCollider(size, pos);

        shapeColour = sf::Color::Red;
        visualShape = new sf::RectangleShape(size);
        visualShape->setFillColor(shapeColour);
        visualShape->setPosition(pos);

        // ------------------------------------------------------------
        // Core
        // ------------------------------------------------------------
        medium = MovementMedium::LAND;

        // ------------------------------------------------------------
        // Attack Profile (ONE SOURCE OF TRUTH)
        // ------------------------------------------------------------
        JumpAttackProfile profile;
        profile.idealHorizontalRange = Constants::TILE_SIZE * 3.0f;
        profile.triggerRange = Constants::TILE_SIZE * 3.5f;
        profile.verticalTolerance = 32.f;

        // ------------------------------------------------------------
        // Sanity Checks (fail fast if misconfigured)
        // ------------------------------------------------------------
        assert(profile.triggerRange > profile.idealHorizontalRange &&
               "Slug: triggerRange must exceed idealHorizontalRange");

        assert(profile.verticalTolerance >= size.y &&
               "Slug: verticalTolerance must be >= slug height");

        // ------------------------------------------------------------
        // Speeds
        // ------------------------------------------------------------
        patrolSpeed = 75.f;
        chaseSpeed = 90.f;
        attackingSpeed = 100.f;

        // ------------------------------------------------------------
        // Combat
        // ------------------------------------------------------------
        verticalAggroTolerance = profile.verticalTolerance;
        aggroRadius = 180.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 2.0f;
        attackCooldown = 0.67f;

        telegraphDuration = 0.33f;
        flashInterval = 0.08f;

        attackDamage = 20;

        // ------------------------------------------------------------
        // Behaviours
        // ------------------------------------------------------------
        patrol = new XYPatrol(leftX, rightX);
        movement = new SlidePulseMovement();
        chase = new DefaultChase();
        attack = new JumpAttack(Constants::TILE_SIZE * 2.0f, profile.idealHorizontalRange);
        attackTrigger = new GroundAttackTrigger(profile.triggerRange);
    }
};

#endif // SLUGENEMY_H
