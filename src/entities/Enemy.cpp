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

int Enemy::dealDamage() const
{
    return attackDamage;
}

// Protected

void Enemy::updatePatrol(float dt, const sf::Vector2f &playerPos)
{
    sf::Vector2f enemyPos = shape->getPosition();
    float distanceEnemyToPlayer = GameUtils::getDistanceBetween(enemyPos, playerPos);
    float distFromPatrol = patrol->getDistFromPatrol(playerPos);

    bool canAggro = canReach(playerPos) &&
                    distanceEnemyToPlayer < aggroRadius &&
                    distFromPatrol <= chaseRadius;

    if (canAggro)
    {
        state = EnemyBehaviourState::CHASE;
        lastChaseProgressTime = 0.f;
        return;
    }

    patrol->patrol(*this, dt);
    movement->move(*this, dt, patrolSpeed);
}

void Enemy::updateChase(float dt, const sf::Vector2f &playerPos)
{
    sf::Vector2f enemyPosition = shape->getPosition();
    float distanceEnemyToPlayer = GameUtils::getDistanceBetween(enemyPosition, playerPos);

    if (distanceEnemyToPlayer < attackRadius &&
        canReach(playerPos))
    {
        state = EnemyBehaviourState::TELEGRAPH;
        telegraphTimer = 0.f;
        lastChaseProgressTime = 0.f;
        return;
    }

    if (distanceEnemyToPlayer > aggroRadius)
    {
        state = EnemyBehaviourState::PATROL;
        telegraphTimer = 0.f;
        lastChaseProgressTime = 0.f;
        return;
    }

    float distFromPatrol = patrol->getDistFromPatrol(enemyPosition);

    // If player is too far, start stall timer
    if (distFromPatrol > chaseRadius ||
        !canReach(playerPos))
    {
        lastChaseProgressTime += dt;

        if (lastChaseProgressTime >= chaseStallDuration)
        {
            // Give up chase
            state = EnemyBehaviourState::PATROL;
            return;
        }
    }
    else
    {
        lastChaseProgressTime = 0.f;
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
    flashAccumulator += dt;

    // Flashing logic
    if (flashAccumulator >= flashInterval)
    {
        flashAccumulator = 0.f;

        if (shape->getFillColor() != shapeColour)
        {
            shape->setFillColor(shapeColour);
        }
        else
        {
            shape->setFillColor(sf::Color::White);
        }
    }

    // Transition to attack
    if (telegraphTimer >= telegraphDuration)
    {
        shape->setFillColor(shapeColour);
        flashAccumulator = 0.f;
        attackTimer = 0.f;
        telegraphTimer = 0.f;

        state = EnemyBehaviourState::ATTACK;
    }
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
    sf::FloatRect bounds = shape->getGlobalBounds();
    sf::Vector2f enemyCenter(bounds.left + bounds.width * 0.5f,
                             bounds.top + bounds.height * 0.5f);

    float dy = std::abs(playerPos.y - enemyCenter.y);
    return dy <= verticalAggroTolerance;
}

void Enemy::applyEnvironmentForces(float dt)
{
    velocity.y += Constants::GRAVITY * dt;
}