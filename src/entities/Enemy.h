#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "constants/EnemyBehaviourState.h"
#include "data/PhysicsResult.h"
#include "entities/enemy/behaviours/attack/AttackBehaviour.h"
#include "entities/enemy/behaviours/movement/MovementBehavior.h"
#include "entities/enemy/behaviours/patrol/PatrolBehaviour.h"

class Enemy
{
protected:
    // --- Core ---
    sf::Shape *shape;
    sf::Color shapeColour;
    EnemyBehaviourState state = EnemyBehaviourState::PATROL;
    int attackDamage = 0;

    // --- Behaviours ---
    PatrolBehavior *patrol = nullptr;
    MovementBehavior *movement = nullptr;
    AttackBehavior *attack = nullptr;

    // --- Movement state ---
    sf::Vector2f velocity = {0.f, 0.f};

    // --- Speeds ---
    float patrolSpeed;
    float chaseSpeed;
    float attackingSpeed;

    // --- Aggro-Attack state ---
    float verticalAggroTolerance; // enemy vertical filter check
    float aggroRadius;            // enemy chases player
    float attackRadius;           // enemy can attack
    float attackCooldown;         // time between attacks
    float attackTimer;            // countdown to attacking

    // --- Telegraphing state ---
    float telegraphTimer = 0.f;
    float telegraphDuration = 0.f;
    float flashInterval = 0.f;
    float flashAccumulator = 0.f;

protected:
    void updatePatrol(float dt, const sf::Vector2f &playerPos);
    void updateChase(float dt, const sf::Vector2f &playerPos);
    void updateTelegraph(float dt, const sf::Vector2f &playerPos);
    void updateAttack(float dt, const sf::Vector2f &playerPos);
    void updateCooldown(float dt, const sf::Vector2f &playerPos);

    bool canReach(const sf::Vector2f &playerPos) const;
    void applyEnvironmentForces(float dt);

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
};

#endif // ENEMY_H
