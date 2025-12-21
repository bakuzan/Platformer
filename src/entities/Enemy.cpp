#include <algorithm>

#include "constants/Constants.h"
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
    switch (state)
    {
    case EnemyBehaviourState::PATROL:
        updatePatrol(dt, playerPos);
        break;

    case EnemyBehaviourState::CHASE:
        updateChase(dt, playerPos);
        break;

    case EnemyBehaviourState::TELEGRAPH:
        updateTelegraph(dt, playerPos);
        break;

    case EnemyBehaviourState::ATTACK:
        updateAttack(dt, playerPos);
        break;

    case EnemyBehaviourState::COOLDOWN:
        updateCooldown(dt, playerPos);
        break;
    }

    applyEnvironmentForces(dt);
}

void Enemy::render(sf::RenderWindow &window)
{
    window.draw(*shape);
}

void Enemy::applyPhysicsResult(PhysicsResult &res)
{
    setPosition(res.position);
    velocity = res.velocity;
}

sf::FloatRect Enemy::getBounds() const
{
    return shape->getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const
{
    return shape->getPosition();
}

void Enemy::setPosition(const sf::Vector2f &update)
{
    return shape->setPosition(update);
}

sf::Vector2f Enemy::getVelocity() const
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

void Enemy::updatePatrol(float dt, const sf::Vector2f &playerPos)
{
    float distanceEnemyToPlayer = GameUtils::getDistanceBetween(shape->getPosition(), playerPos);

    if (distanceEnemyToPlayer < aggroRadius &&
        canReach(playerPos))
    {
        state = EnemyBehaviourState::CHASE;
        return;
    }

    patrol->patrol(*this, dt);
    movement->move(*this, dt, patrolSpeed);
}

void Enemy::updateChase(float dt, const sf::Vector2f &playerPos)
{
    float distanceEnemyToPlayer = GameUtils::getDistanceBetween(shape->getPosition(), playerPos);

    if (distanceEnemyToPlayer < attackRadius)
    {
        state = EnemyBehaviourState::TELEGRAPH;
        telegraphTimer = 0.f;
        return;
    }

    sf::Vector2f dir = playerPos - shape->getPosition();
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    dir /= len;

    velocity = dir;

    movement->move(*this, dt, chaseSpeed);
}

void Enemy::updateTelegraph(float dt, const sf::Vector2f &playerPos)
{
    (void)playerPos;

    telegraphTimer += dt;

    if (telegraphTimer >= telegraphDuration)
    {
        state = EnemyBehaviourState::ATTACK;
        attackTimer = 0.f;
        telegraphTimer = 0.f;
        return;
    }

    // TODO shake, flash, freeze, etc.
}

void Enemy::updateAttack(float dt, const sf::Vector2f &playerPos)
{
    attack->attack(*this, dt, playerPos, attackingSpeed);

    if (attack->isFinished())
    {
        state = EnemyBehaviourState::COOLDOWN;
        attackTimer = attackCooldown;
    }
}

void Enemy::updateCooldown(float dt, const sf::Vector2f &playerPos)
{
    (void)playerPos;

    attackTimer -= dt;

    if (attackTimer <= 0.f)
    {
        state = EnemyBehaviourState::CHASE;
        return;
    }

    // TODO...Consider doing something...but idle for now
}

bool Enemy::canReach(const sf::Vector2f &playerPos) const
{
    float dy = std::abs(playerPos.y - shape->getPosition().y);
    return dy <= verticalAggroTolerance;
}

void Enemy::applyEnvironmentForces(float dt)
{
    velocity.y += Constants::GRAVITY * dt;
}