#include <algorithm>

#include "constants/Constants.h"
#include "components/TileMap.h"
#include "utils/GameUtils.h"

#include "Enemy.h"

Enemy::Enemy()
{
    // Constructor
}

Enemy::~Enemy()
{
    delete patrol;
    delete movement;
    delete attack;
    delete chase;
    delete attackTrigger;
}

// Publics

void Enemy::update(float dt, const sf::Vector2f &playerPos, const TileMap &map)
{
    if (damageFlashTimer > 0.f)
    {
        damageFlashTimer -= dt;
    }

    switch (state)
    {
    case EnemyBehaviourState::PATROL:
        updatePatrol(dt, playerPos, map);
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

    // Damage flash
    if (damageFlashTimer > 0.f)
    {
        visualShape->setFillColor(sf::Color::White);
    }
    else
    {
        visualShape->setFillColor(shapeColour);
    }

    // Health bar
    sf::Vector2f barPos;
    barPos.x = getBounds().left + (getBounds().width / 2.f) - 25.f;
    barPos.y = getBounds().top - 15.f;

    healthBar.update(health, maxHealth, barPos);
}

void Enemy::render(sf::RenderWindow &window)
{
    renderDebugCollider(window);

    window.draw(*visualShape);
}

void Enemy::renderHealthBar(sf::RenderWindow &window)
{
    healthBar.render(window);
}

void Enemy::applyPhysicsResult(PhysicsResult &res)
{
    sf::Vector2f before = velocity;

    setPosition(res.position);
    velocity = res.velocity;
    currentTileType = res.tileProps.type;
    grounded = res.grounded;

    // When attacking we need to see if something iterrupted us
    if (state == EnemyBehaviourState::ATTACK)
    {
        bool blockedX = (before.x != 0.f && res.velocity.x == 0.f);
        bool blockedY = (before.y < 0.f && res.velocity.y == 0.f);

        if (blockedX || blockedY)
        {
            attack->abortAttack();
            state = EnemyBehaviourState::COOLDOWN;
            attackTimer = attackCooldown;
            velocity = {0.f, 0.f};
        }
    }
}

sf::FloatRect Enemy::getBounds() const
{
    return collider.getGlobalBounds();
}

sf::RectangleShape Enemy::getCollider() const
{
    return collider;
}

std::vector<sf::FloatRect> Enemy::getExtraColliders() const
{
    return {};
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

float Enemy::dealDamage() const
{
    return attackDamage;
}

void Enemy::takeDamage(float damage)
{
    updateHealth(-damage);

    damageFlashTimer = DAMAGE_FLASH_DURATION;
}

bool Enemy::isDead() const
{
    return health <= 0;
}

bool Enemy::isGrounded() const
{
    return grounded;
}

bool Enemy::isSwimming() const
{
    return currentTileType == TileCategory::WATER;
}

EntityCapabilities Enemy::getCapabilities() const
{
    if (state != EnemyBehaviourState::ATTACK)
    {
        return EntityCapabilities::create(false, false);
    }

    // If we are above the attack start Y, ignore top-only
    float feetY = getBounds().top + getBounds().height;
    bool ignoreTopPlatforms = feetY > attackStartY;
    return EntityCapabilities::create(false, ignoreTopPlatforms);
}

// Protected

void Enemy::updatePatrol(float dt, const sf::Vector2f &playerPos, const TileMap &map)
{
    sf::Vector2f enemyCenter = GameUtils::getCentre(collider);
    float distanceEnemyToPlayer = GameUtils::getDistanceBetween(collider.getPosition(), playerPos);
    bool inRange = canReach(playerPos) &&
                   distanceEnemyToPlayer < aggroRadius;

    bool hasLoS = false;
    if (inRange)
    {
        hasLoS = map.hasLineOfSight(enemyCenter, playerPos);
    }

    if (inRange && hasLoS)
    {
        state = EnemyBehaviourState::CHASE;
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
        return;
    }

    float distanceEnemyToPlayer = GameUtils::getDistanceBetween(collider.getPosition(), playerPos);

    if (distanceEnemyToPlayer > chaseRadius ||
        !canReach(playerPos))
    {
        state = EnemyBehaviourState::PATROL;
        telegraphTimer = 0.f;
        return;
    }

    // Water Check: If a water creature breaches the surface, kill movement
    if (medium == MovementMedium::WATER &&
        !isSwimming())
    {
        return;
    }

    // Relentless pursuit
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

        if (damageFlashTimer <= 0.f)
        {
            if (visualShape->getFillColor() != shapeColour)
            {
                visualShape->setFillColor(shapeColour);
            }
            else
            {
                visualShape->setFillColor(Constants::attackTelegraphColour);
            }
        }
    }

    // Transition to attack
    if (telegraphTimer >= telegraphDuration)
    {
        visualShape->setFillColor(shapeColour);
        flashAccumulator = 0.f;
        attackTimer = 0.f;
        telegraphTimer = 0.f;

        // Grab startingY, needed for platforming decisions
        sf::FloatRect b = getBounds();
        attackStartY = b.top + b.height;

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

    if (medium == MovementMedium::AIR)
    {
        sf::Vector2f v = getVelocity();
        v.x *= 0.98f; // Gentle air drag
        v.y *= 0.98f;
        setVelocity(v);
    }
}

bool Enemy::canReach(const sf::Vector2f &playerPos) const
{
    sf::Vector2f enemyCenter = GameUtils::getCentre(collider);
    float dy = std::abs(playerPos.y - enemyCenter.y);

    return dy <= verticalAggroTolerance;
}

void Enemy::applyEnvironmentForces(float dt)
{
    switch (medium)
    {
    case MovementMedium::LAND:
        velocity.y += Constants::GRAVITY * dt;
        // Die if fall into water
        if (currentTileType == TileCategory::WATER)
        {
            health = 0;
        }
        break;

    case MovementMedium::AIR:
        // Never apply gravity (fliers)
        break;

    case MovementMedium::WATER:
        if (!isSwimming())
        {
            velocity.x *= 0.1f;
            velocity.y += Constants::GRAVITY * dt;
        }

        break;
    default:
        break;
    }
}

void Enemy::updateHealth(float update)
{
    int newHealth = static_cast<int>(std::round(health + update));
    health = std::max(0, std::min(maxHealth, newHealth));
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