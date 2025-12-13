#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "entities/enemy/behaviours/attack/AttackBehaviour.h"
#include "entities/enemy/behaviours/movement/MovementBehavior.h"
#include "entities/enemy/behaviours/patrol/PatrolBehaviour.h"

class Enemy
{
protected:
    // --- Core ---
    sf::Shape *shape;

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
    float aggroRadius;    // enemy chases player
    float attackRadius;   // enemy can attack
    float attackCooldown; // time between attacks
    float attackTimer;    // countdown to attacking

    float telegraphTimer = 0.f;
    float telegraphDuration = 0.f;

protected:
    void handleAggro(float dt, const sf::Vector2f &playerPos);
    void tryAttack(float dt, const sf::Vector2f &playerPos);

public:
    Enemy();
    ~Enemy();

    void update(float dt, const sf::Vector2f &playerPos);
    void render(sf::RenderWindow &window);
};

#endif // ENEMY_H
