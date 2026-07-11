#include <vector>

#include "constants/Constants.h"
#include "components/PhysicsSystem.h"
#include "data/PhysicsResult.h"
#include "utils/GameUtils.h"
#include "utils/InputUtils.h"

#include "Player.h"

Player::Player(float size)
    : playerSize(size)
{
    // TODO Implement the real player texture code!
    int s = static_cast<int>(size);
    sf::Image img;

    img.create(s, s, Constants::playerColour);
    texture.loadFromImage(img);
    sprite.setTexture(texture);
}

Player::~Player()
{
    // Destructor
}

// Publics

void Player::update(float dt)
{
    previousBounds = sprite.getGlobalBounds();

    // Drop-through timer
    if (dropThroughTimer > 0.f)
    {
        dropThroughTimer -= dt;
        if (dropThroughTimer < 0.f)
        {
            dropThroughTimer = 0.f;
        }
    }

    // Invincible cooldown
    invincibleTime = std::max(0.f, invincibleTime - dt);
    updateInvincibilityFeedback(dt);

    // Dash timers
    if (dashCooldown > 0.f)
    {
        dashCooldown = std::max(0.f, dashCooldown - dt);
    }

    if (isDashing)
    {
        dashTime -= dt;
        if (dashTime <= 0.f)
        {
            isDashing = false;
        }
    }

    // Smash cooldown
    if (smashCooldown > 0.f)
    {
        smashCooldown = std::max(0.f, smashCooldown - dt);
    }

    if (isSmashing &&
        (isGrounded || isSwimming()))
    {
        isSmashing = false;
    }

    // Wall stick check
    if (!isGrounded && !isSmashing && !isSwimming() &&
        hasAbility(PlayerAbility::WALL_CLIMB))
    {
        if ((touchingClimbableWallLeft && !facingRight) ||
            (touchingClimbableWallRight && facingRight))
        {
            isWallSticking = true;
            jumpsUsed = 0;
        }
        else
        {
            isWallSticking = false;
        }
    }
    else
    {
        isWallSticking = false;
    }

    if (fireCooldown > 0.0f)
    {
        fireCooldown -= dt;
    }

    // Apply environment forces
    if (!isDashing && !isSmashing)
    {
        applyEnvironmentForces(dt);
    }
}

void Player::render(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

void Player::applyPhysicsResult(PhysicsResult &res)
{
    setPosition(res.position);
    velocity = res.velocity;
    isGrounded = res.grounded;
    currentTileType = res.tileProps.type;

    touchingClimbableWallLeft = res.touchingClimbableWallLeft;
    touchingClimbableWallRight = res.touchingClimbableWallRight;

    if (isGrounded)
    {
        jumpsUsed = 0;
    }
}

void Player::setSpawnPosition(sf::Vector2f pos)
{
    setPosition(pos);
    velocity = {0.f, 0.f};
    isGrounded = true;
    isWallSticking = false;
    jumpsUsed = 0;
}

void Player::setPosition(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

sf::FloatRect Player::getPreviousBounds() const
{
    return previousBounds;
}

sf::FloatRect Player::getBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getCenter() const
{
    return GameUtils::getCentre(sprite);
}

sf::Sprite Player::getSprite() const
{
    return sprite;
}

sf::Vector2f Player::getVelocity() const
{
    return velocity;
}

bool Player::isTangible() const
{
    return !isInvincible();
}

EntityCapabilities Player::getCapabilities() const
{
    return EntityCapabilities::create(
        isSmashing,
        dropThroughTimer > 0.f);
}

void Player::setAbility(PlayerAbility ability)
{
    abilities.insert(ability);
}

bool Player::hasAbility(PlayerAbility ability) const
{
    return abilities.contains(ability);
}

std::vector<PlayerAbility> Player::getCurrentAbilities() const
{
    return std::vector<PlayerAbility>(abilities.begin(), abilities.end());
}

PlayerState Player::getPlayerState() const
{
    if (isSwimming())
    {
        return PlayerState::SWIMMING;
    }
    else if (isSmashing)
    {
        return PlayerState::SMASHING;
    }
    else if (!isGrounded)
    {
        return PlayerState::AIRBORNE;
    }

    return PlayerState::GROUNDED;
}

int Player::getMaxHealth() const
{
    return maxHealth;
}

int Player::getHealth() const
{
    return health;
}

void Player::healDamage(float heal)
{
    updateHealth(heal);
}

void Player::takeDamage(float damage)
{
    if (!isInvincible())
    {
        updateHealth(-damage);
        invincibleTime = Constants::INVINCIBLE_TIME;
    }
}

bool Player::isDead() const
{
    return health <= 0;
}

bool Player::canShoot() const
{
    return fireCooldown <= 0.0f &&
           !isDead() &&
           !isSmashing;
}

ProjectileType Player::getCurrentAmmoType() const
{
    return currentAmmo;
}

void Player::resetFireCooldown(float cooldownTime)
{
    fireCooldown = cooldownTime;
}

void Player::handleHorizontalInput(float dt, bool leftHeld, bool rightHeld)
{
    (void)dt;

    const float moveSpeed = isSwimming()
                                ? Constants::MOVE_SPEED_WATER
                                : Constants::MOVE_SPEED_GROUND;

    if (isDashing)
    {
        const float dashSpeed = isSwimming()
                                    ? Constants::DASH_SPEED_WATER
                                    : Constants::DASH_SPEED_GROUND;

        velocity.x = facingRight ? dashSpeed : -dashSpeed;
    }
    else if (leftHeld)
    {
        velocity.x = -moveSpeed;
        facingRight = false;
    }
    else if (rightHeld)
    {
        velocity.x = moveSpeed;
        facingRight = true;
    }
    else
    {
        velocity.x = 0.f;
    }
}

void Player::handleVerticalInput(float dt, bool upHeld, bool downHeld)
{
    // Jump buffer
    jumpBufferTime = std::max(0.f, jumpBufferTime - dt);

    if (isDashing)
    {
        velocity.y = 0.f; // Cancel vertical velocity for a "clean" dash
    }
    else if (isSwimming())
    {
        if (jumpBufferTime > 0.f)
        {
            velocity.y = -Constants::WATER_JUMP_STRENGTH;
            jumpBufferTime = 0.f;
            waterJumpLock = 0.1f;
        }
        else if (waterJumpLock <= 0.f)
        {
            if (upHeld)
            {
                velocity.y = -Constants::SWIM_SPEED_VERTICAL;
            }
            else if (downHeld)
            {
                velocity.y = Constants::SWIM_SPEED_VERTICAL;
            }
            else
            {
                velocity.y = Constants::SINK_SPEED;
            }
        }
    }
    else if (isSmashing)
    {
        velocity.y = Constants::SMASH_SPEED;
    }
    else
    {
        // Coyote time
        if (isGrounded)
        {
            coyoteTime = Constants::COYOTE_TIME;
        }
        else
        {
            coyoteTime = std::max(0.f, coyoteTime - dt);
        }

        // Jumping
        if (jumpBufferTime > 0.f)
        {
            bool canGroundJump = (coyoteTime > 0.f && jumpsUsed == 0);
            bool canAirJump = (!isGrounded &&
                               jumpsUsed < maxJumps &&
                               hasAbility(PlayerAbility::DOUBLE_JUMP));

            if (canGroundJump ||
                canAirJump)
            {
                if (isWallSticking)
                {
                    velocity.x = touchingClimbableWallLeft
                                     ? Constants::WALL_JUMP_PUSH
                                     : -Constants::WALL_JUMP_PUSH;
                    isWallSticking = false;
                }

                velocity.y = -Constants::JUMP_STRENGTH;
                isGrounded = false;
                jumpBufferTime = 0.f;
                jumpsUsed++;
            }
        }
    }

    // Timers
    waterJumpLock = std::max(0.f, waterJumpLock - dt);
}

void Player::onJumpPressed(bool dropThrough)
{
    if (dropThrough)
    {
        dropThroughTimer = 0.18f;
    }
    else
    {
        if (!isJumpHeld)
        {
            jumpBufferTime = Constants::JUMP_BUFFER_TIME;
        }

        isJumpHeld = true;
    }
}

void Player::onJumpReleased()
{
    isJumpHeld = false;
}

void Player::onDashPressed()
{
    if (dashCooldown <= 0.f &&
        !isDashing)
    {
        isDashing = true;
        dashTime = Constants::DASH_DURATION;
        dashCooldown = Constants::DASH_COOLDOWN;
    }
}

void Player::onSmashPressed()
{
    if (smashCooldown <= 0.f &&
        !isGrounded &&
        !isSmashing &&
        !isSwimming())
    {
        isSmashing = true;
        velocity.y = Constants::SMASH_SPEED;
        smashCooldown = Constants::SMASH_COOLDOWN;
    }
}

// Privates

bool Player::isSwimming() const
{
    return currentTileType == TileCategory::WATER;
}

bool Player::isInvincible() const
{
    return invincibleTime > 0.f;
}

void Player::applyEnvironmentForces(float dt)
{
    // Gravity
    if (!isSwimming() && !isGrounded)
    {
        if (!isWallSticking)
        {
            velocity.y += Constants::GRAVITY * dt;
        }
        else
        {
            velocity.y = 0.0f;
        }
    }
}

void Player::updateHealth(float update)
{
    int newHealth = static_cast<int>(std::round(health + update));
    health = std::max(0, std::min(maxHealth, newHealth));
}

void Player::updateInvincibilityFeedback(float dt)
{
    if (invincibleTime <= 0.f)
    {
        sprite.setColor(sf::Color::Yellow);
        return;
    }

    invincibleTime -= dt;

    // Compute dynamic flicker interval
    float t = 1.f - (invincibleTime / Constants::INVINCIBLE_TIME);
    float eased = t * t * (3.f - 2.f * t);

    float flashInterval = Constants::MIN_FLASH_INTERVAL + (Constants::MAX_FLASH_INTERVAL - Constants::MIN_FLASH_INTERVAL) * eased;

    flashAccumulator += dt;

    if (flashAccumulator >= flashInterval)
    {
        flashAccumulator = 0.f;

        sf::Color c = sprite.getColor();

        // Toggle alpha between visible and invisible
        if (c.a == 255)
        {
            c.a = 40;
        }
        else
        {
            c.a = 255;
        }

        sprite.setColor(c);
    }
}
