#ifndef FLIERENEMY_H
#define FLIERENEMY_H

#include <SFML/Graphics.hpp>
#include <cassert>

#include "constants/Constants.h"
#include "constants/MovementMedium.h"

#include "data/profiles/FlierAttackProfile.h"

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
              // ------------------------------------------------------------
              // Collider + Visual
              // ------------------------------------------------------------
              sf::Vector2f colliderSize(16.f, 16.f);
              setCollider(colliderSize, pos);

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

              // ------------------------------------------------------------
              // Core
              // ------------------------------------------------------------
              medium = MovementMedium::AIR;
              int startingHealth = 30;
              maxHealth = startingHealth;
              health = startingHealth;

              // ------------------------------------------------------------
              // Attack Profile (single source of truth)
              // ------------------------------------------------------------
              FlierAttackProfile profile;
              profile.horizontalRange = Constants::TILE_SIZE * 3.0f;
              profile.verticalRange = Constants::TILE_SIZE * 2.0f;
              profile.diveSpeed = 280.f;

              // ------------------------------------------------------------
              // Sanity Checks
              // ------------------------------------------------------------
              assert(profile.horizontalRange > 0.f &&
                     "Flier: horizontalRange must be positive");

              assert(profile.verticalRange > 0.f &&
                     "Flier: verticalRange must be positive");

              // ------------------------------------------------------------
              // Speeds
              // ------------------------------------------------------------
              patrolSpeed = 65.f; // quick flapping
              chaseSpeed = 130.f; // faster flapping
              attackingSpeed = profile.diveSpeed;

              // ------------------------------------------------------------
              // Combat
              // ------------------------------------------------------------
              verticalAggroTolerance = colliderSize.y * 6.f;
              aggroRadius = 250.f;
              chaseRadius = aggroRadius + (aggroRadius / 3.0f);
              chaseStallDuration = 1.0f;
              attackCooldown = 0.5f;

              telegraphDuration = 0.2f;
              flashInterval = 0.08f;

              attackDamage = 15.f;

              // ------------------------------------------------------------
              // Behaviours
              // ------------------------------------------------------------
              patrol = new AirPatrol({leftX, pos.y}, {rightX, pos.y});
              movement = new FlappingMovement();
              chase = new FlyingChase(pos.y);
              attack = new DivingAttack();
              attackTrigger = new FlyingAttackTrigger(profile.horizontalRange,
                                                      profile.verticalRange);
       }
};

#endif // FLIERENEMY_H
