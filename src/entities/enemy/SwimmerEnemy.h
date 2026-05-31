#ifndef SWIMMERENEMY_H
#define SWIMMERENEMY_H

#include <SFML/Graphics.hpp>
#include <cassert>

#include "constants/Constants.h"
#include "constants/MovementMedium.h"
#include "data/profiles/SwimmerProfile.h"

#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/SwimMovement.h"
#include "entities/enemy/behaviours/patrol/AreaPatrol.h"
#include "entities/enemy/behaviours/chase/AquaticChase.h"
#include "entities/enemy/behaviours/attack/OmniDirectionalChargeAttack.h"
#include "entities/enemy/behaviours/attackTrigger/RadialAttackTrigger.h"

class SwimmerEnemy : public Enemy
{
public:
    SwimmerEnemy(const sf::Vector2f &pos,
                 float leftX, float rightX,
                 float topY, float bottomY)
    {
        // ------------------------------------------------------------
        // Collider + Visual
        // ------------------------------------------------------------
        sf::Vector2f size(20.f, 20.f);
        setCollider(size, pos);

        shapeColour = sf::Color::Red;
        visualShape = new sf::CircleShape(size.x / 2.f);
        visualShape->setFillColor(shapeColour);
        visualShape->setPosition(pos);

        // ------------------------------------------------------------
        // Core
        // ------------------------------------------------------------
        medium = MovementMedium::WATER;

        // ------------------------------------------------------------
        // Attack Profile (ONE SOURCE OF TRUTH)
        // ------------------------------------------------------------
        SwimmerProfile profile;
        profile.chargeDuration = 0.75f;
        profile.skidDuration = 0.25f;
        profile.triggerRange = Constants::TILE_SIZE * 4.0f;
        profile.waveFrequency = 3.0f;
        profile.waveAmplitude = 0.4f;

        // ------------------------------------------------------------
        // Speeds
        // ------------------------------------------------------------
        patrolSpeed = 70.f;
        chaseSpeed = 125.f;
        attackingSpeed = 275.f;

        // ------------------------------------------------------------
        // Sanity Checks (Fail fast if misconfigured)
        // ------------------------------------------------------------
        assert(rightX > leftX && "Swimmer: rightX must be greater than leftX");
        assert(bottomY > topY && "Swimmer: bottomY must be greater than topY");
        assert(profile.chargeDuration > 0.f && "Swimmer: Charge duration must be positive");

        // ------------------------------------------------------------
        // Combat
        // ------------------------------------------------------------
        verticalAggroTolerance = size.y * 5.0f;
        aggroRadius = 180.f;
        chaseRadius = aggroRadius + (aggroRadius / 3.0f);
        chaseStallDuration = 1.0f;
        attackCooldown = 0.67f;

        telegraphDuration = 0.24f;
        flashInterval = 0.08f;
        attackDamage = 20;

        // ------------------------------------------------------------
        // Behaviours
        // ------------------------------------------------------------
        patrol = new AreaPatrol(leftX, rightX, topY, bottomY);
        movement = new SwimMovement(profile.waveFrequency, profile.waveAmplitude);
        chase = new AquaticChase();
        attack = new OmniDirectionalChargeAttack(profile.chargeDuration, profile.skidDuration);
        attackTrigger = new RadialAttackTrigger(profile.triggerRange);
    }
};

#endif // SWIMMERENEMY_H