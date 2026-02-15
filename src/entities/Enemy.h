#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "constants/EnemyBehaviourState.h"
#include "constants/MovementMedium.h"
#include "constants/TileCategory.h"
#include "data/EntityCapabilities.h"
#include "data/PhysicsResult.h"
#include "entities/enemy/behaviours/attack/AttackBehaviour.h"
#include "entities/enemy/behaviours/attackTrigger/AttackTriggerBehaviour.h"
#include "entities/enemy/behaviours/chase/ChaseBehaviour.h"
#include "entities/enemy/behaviours/movement/MovementBehaviour.h"
#include "entities/enemy/behaviours/patrol/PatrolBehaviour.h"

class Enemy
{
protected:
    // --- Core ---
    sf::RectangleShape debugCollider;
    sf::RectangleShape collider;
    sf::Shape *visualShape;
    sf::Vector2f visualOffset;

    sf::Color shapeColour;
    EnemyBehaviourState state = EnemyBehaviourState::PATROL;
    MovementMedium medium = MovementMedium::LAND;
    TileCategory currentTileType = TileCategory::EMPTY;

    int health = 10;
    int attackDamage = 0;

    // --- Behaviours ---
    PatrolBehaviour *patrol = nullptr;
    MovementBehaviour *movement = nullptr;
    AttackBehaviour *attack = nullptr;
    ChaseBehaviour *chase = nullptr;
    AttackTriggerBehaviour *attackTrigger = nullptr;

    // --- Movement state ---
    sf::Vector2f velocity = {0.f, 0.f};
    bool ignoreGravity = false;
    float attackStartY = 0.f;

    // --- Speeds ---
    float patrolSpeed;
    float chaseSpeed;
    float attackingSpeed;

    // --- Aggro-Attack state ---
    float verticalAggroTolerance; // enemy vertical filter check

    float aggroRadius; // enemy chases player
    float chaseRadius; // enemy chase range
    float lastChaseProgressTime = 0.f;
    float chaseStallDuration = 0.f;

    float attackCooldown; // time between attacks
    float attackTimer;    // countdown to attacking

    // --- Telegraphing state ---
    float telegraphTimer = 0.f;
    float telegraphDuration = 0.f;
    float flashInterval = 0.f;
    float flashAccumulator = 0.f;

protected:
    bool isSwimming() const;

    void updatePatrol(float dt, const sf::Vector2f &playerPos);
    void updateChase(float dt, const sf::Vector2f &playerPos);
    void updateTelegraph(float dt, const sf::Vector2f &playerPos);
    void updateAttack(float dt, const sf::Vector2f &playerPos);
    void updateCooldown(float dt, const sf::Vector2f &playerPos);

    void applyEnvironmentForces(float dt);

    void setCollider(const sf::Vector2f &size,
                     const sf::Vector2f &position);

    void renderDebugCollider(sf::RenderWindow &window);

public:
    Enemy();
    ~Enemy();

    void update(float dt, const sf::Vector2f &playerPos);
    void render(sf::RenderWindow &window);

    void applyPhysicsResult(PhysicsResult &res);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f &update);
    sf::Vector2f getVelocity() const;
    void setVelocity(const sf::Vector2f &update);
    void move(const sf::Vector2f &offset);

    int dealDamage() const;
    void takeDamage(int damage);
    bool isDead() const;

    bool canReach(const sf::Vector2f &playerPos) const;
    EntityCapabilities getCapabilities() const;
};

#endif // ENEMY_H
