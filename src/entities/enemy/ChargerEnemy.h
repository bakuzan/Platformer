#ifndef CHARGERENEMY_H
#define CHARGERENEMY_H

#include <SFML/Graphics.hpp>
#include <cassert>

#include "constants/Constants.h"
#include "constants/MovementMedium.h"

#include "data/profiles/ChargeAttackProfile.h"

#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/LumberDriftMovement.h"
#include "entities/enemy/behaviours/patrol/XYPatrol.h"
#include "entities/enemy/behaviours/chase/DefaultChase.h"
#include "entities/enemy/behaviours/attack/ChargeAttack.h"
#include "entities/enemy/behaviours/attackTrigger/GroundAttackTrigger.h"

class ChargerEnemy : public Enemy
{
public:
    ChargerEnemy(const sf::Vector2f &pos, float leftX, float rightX)
    {
        // ------------------------------------------------------------
        // Collider + Visual
        // ------------------------------------------------------------
        sf::Vector2f size(60.f, 60.f);
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
        ChargeAttackProfile profile;
        profile.triggerRange = Constants::TILE_SIZE * 5.0f;
        profile.verticalTolerance = size.y;
        profile.chargeDuration = 0.8f;
        profile.skidDuration = 0.2f;

        // ------------------------------------------------------------
        // Sanity Checks (fail fast if misconfigured)
        // ------------------------------------------------------------
        assert(profile.chargeDuration > 0.f && "Charger: Charge duration must be > 0");
        assert(profile.triggerRange > 0.f && "Charger: Trigger range must be > 0");

        // ------------------------------------------------------------
        // Speeds
        // ------------------------------------------------------------
        patrolSpeed = 40.f;     // slow crawl
        chaseSpeed = 100.f;     // faster when aggro'd
        attackingSpeed = 350.f; // charge horizontal max speed

        // ------------------------------------------------------------
        // Combat
        // ------------------------------------------------------------
        verticalAggroTolerance = profile.verticalTolerance;
        aggroRadius = 224.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 3.0f;
        attackCooldown = 1.f;

        telegraphDuration = 0.33f;
        flashInterval = 0.08f;

        attackDamage = 40;

        // ------------------------------------------------------------
        // Behaviours
        // ------------------------------------------------------------
        patrol = new XYPatrol(leftX, rightX);
        movement = new LumberDriftMovement(1.2f, 0.075f, 0.85f);
        chase = new DefaultChase();
        attack = new ChargeAttack(profile.chargeDuration, profile.skidDuration);
        attackTrigger = new GroundAttackTrigger(profile.triggerRange);
    }
};

#endif // CHARGERENEMY_H