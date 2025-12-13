#include "utils/GameUtils.h"

#include "Enemy.h"

Enemy::Enemy()
{
    // Constructor
}

Enemy::~Enemy()
{
    // Destructor
}

// Publics

void Enemy::update(float dt, const sf::Vector2f &playerPos)
{
    float distanceEnemyToPlayer = GameUtils::getDistanceBetween(shape->getPosition(), playerPos);

    // Cooldown
    if (attackTimer > 0.f)
    {
        attackTimer -= dt;
    }

    if (distanceEnemyToPlayer < aggroRadius)
    {
        handleAggro(dt, playerPos);
    }
    else
    {
        patrol->patrol(*this, dt);
        movement->move(*this, dt, patrolSpeed);
    }
}

void Enemy::render(sf::RenderWindow &window)
{
    window.draw(*shape);
}

// Protected

void Enemy::handleAggro(float dt, const sf::Vector2f &playerPos)
{
    if (dist < attackRadius)
    {
        tryAttack(dt, playerPos);
    }
    else
    {
        // Chasing
        sf::Vector2f dir = playerPos - shape->getPosition();
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        dir /= len;

        velocity = dir;

        movement->move(*this, dt, chaseSpeed);
    }
}

void Enemy::tryAttack(float dt, const sf::Vector2f &playerPos)
{
    if (attackTimer > 0.f)
    {
        return;
    }

    // Telegraph first
    telegraphTimer += dt;
    if (telegraphTimer < telegraphDuration)
    {
        // TODO maybe flash, shake, etc.
        return;
    }

    // Do attack
    attack->attack(*this, dt, playerPos, attackingSpeed);

    // Reset timers
    telegraphTimer = 0.f;
    attackTimer = attackCooldown;
}
