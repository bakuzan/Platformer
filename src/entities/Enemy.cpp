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

    bool canAggro = (distanceEnemyToPlayer < aggroRadius) &&
                    canReach(playerPos);

    if (canAggro)
    {
        handleAggro(dt, distanceEnemyToPlayer, playerPos);
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

sf::Vector2f Enemy::getPosition()
{
    return shape->getPosition();
}

void Enemy::setPosition(const sf::Vector2f &update)
{
    return shape->setPosition(update);
}

sf::Vector2f Enemy::getVelocity()
{
    return velocity;
}

void Enemy::setVelocity(const sf::Vector2f &update)
{
    velocity = update;
}

void Enemy::move(const sf::Vector2f &offset)
{
    shape->move(offset);
}

// Protected

bool Enemy::canReach(const sf::Vector2f &playerPos) const
{
    float dy = std::abs(playerPos.y - shape->getPosition().y);
    return dy <= verticalAggroTolerance;
}

void Enemy::handleAggro(float dt, float distanceToPlayer,
                        const sf::Vector2f &playerPos)
{
    if (distanceToPlayer < attackRadius)
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
