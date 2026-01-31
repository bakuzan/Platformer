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
    renderDebugCollider(window);

    window.draw(*visualShape);
}

void Enemy::applyPhysicsResult(PhysicsResult &res)
{
    setPosition(res.position);
    velocity = res.velocity;
    currentTileType = res.tileProps.type;
}

sf::FloatRect Enemy::getBounds() const
{
    return collider.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const
{
    return collider.getPosition();
}

void Enemy::setPosition(const sf::Vector2f &update)
{
    collider.setPosition(update);
    visualShape->setPosition(update + visualOffset);
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
    collider.move(offset);
    visualShape->setPosition(collider.getPosition() + visualOffset);
}

int Enemy::dealDamage() const
{
    return attackDamage;
}

// Protected

bool Enemy::isSwimming() const
{
    return currentTileType == TileCategory::WATER;
}

void Enemy::updatePatrol(float dt, const sf::Vector2f &playerPos)
{
    sf::Vector2f enemyPos = collider.getPosition();
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
    if (attackTrigger->shouldAttack(*this, playerPos))
    {
        state = EnemyBehaviourState::TELEGRAPH;
        telegraphTimer = 0.f;
        lastChaseProgressTime = 0.f;
        return;
    }

    sf::Vector2f enemyPosition = collider.getPosition();
    float distanceEnemyToPlayer = GameUtils::getDistanceBetween(enemyPosition, playerPos);

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

    chase->chase(*this, dt, playerPos);
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

        if (visualShape->getFillColor() != shapeColour)
        {
            visualShape->setFillColor(shapeColour);
        }
        else
        {
            visualShape->setFillColor(sf::Color::White);
        }
    }

    // Transition to attack
    if (telegraphTimer >= telegraphDuration)
    {
        visualShape->setFillColor(shapeColour);
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
    sf::FloatRect bounds = collider.getGlobalBounds();
    sf::Vector2f enemyCenter(bounds.left + bounds.width * 0.5f,
                             bounds.top + bounds.height * 0.5f);

    float dy = std::abs(playerPos.y - enemyCenter.y);
    return dy <= verticalAggroTolerance;
}

void Enemy::applyEnvironmentForces(float dt)
{
    switch (medium)
    {
    case MovementMedium::LAND:
        velocity.y += Constants::GRAVITY * dt;
        break;

    case MovementMedium::AIR:
        // Never apply gravity (fliers)
        break;

    case MovementMedium::WATER:
        if (!isSwimming())
        {
            velocity.x *= 0.1f;
            velocity.y = Constants::GRAVITY * dt;
        }

        break;
    default:
        break;
    }
}

void Enemy::setCollider(const sf::Vector2f &size,
                        const sf::Vector2f &position)
{
    collider.setSize(size);
    collider.setPosition(position);
}

void Enemy::renderDebugCollider(sf::RenderWindow &window)
{
    debugCollider.setSize(collider.getSize());
    debugCollider.setPosition(collider.getPosition());
    debugCollider.setFillColor(sf::Color::Transparent);
    debugCollider.setOutlineColor(sf::Color::Yellow);
    debugCollider.setOutlineThickness(1.f);
    window.draw(debugCollider);
}