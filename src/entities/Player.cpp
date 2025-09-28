#include "constants/Constants.h"
#include "data/PhysicsResult.h"
#include "utils/InputUtils.h"
#include "Player.h"

Player::Player()
{
    // TODO Implement the real player texture code!
    sf::Image img;
    img.create(32, 32, sf::Color::Yellow);
    texture.loadFromImage(img);
    sprite.setTexture(texture);
}

Player::~Player()
{
    // Destructor
}

// Publics

void Player::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed &&
        InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Space}))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            // signal that you are dropping from platform
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
    else if (event.type == sf::Event::KeyReleased &&
             InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Space}))
    {
        isJumpHeld = false;
    }
}

void Player::update(float dt, const PhysicsSystem &physics)
{
    handleHorizontalInput(dt);
    handleVerticalInput(dt);

    // Drop-through timer
    if (dropThroughTimer > 0.f)
    {
        dropThroughTimer -= dt;
        if (dropThroughTimer < 0.f)
        {
            dropThroughTimer = 0.f;
        }
    }

    applyEnvironmentForces(dt);

    // Physics step
    PhysicsResult res = physics.moveAndCollide(
        getBounds(),
        velocity,
        dt,
        dropThroughTimer > 0.f);

    setPosition(res.position);
    velocity = res.velocity;
    isGrounded = res.grounded;
    currentTileType = res.tileType;
}

void Player::render(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

void Player::setSpawnPosition(sf::Vector2f pos)
{
    setPosition(pos);
    velocity = {0.f, 0.f};
    isGrounded = true;
}

void Player::setPosition(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

sf::FloatRect Player::getBounds() const
{
    return sprite.getGlobalBounds();
}

// Privates

void Player::handleHorizontalInput(float dt)
{
    (void)dt;

    const float moveSpeed = isSwimming()
                                ? Constants::MOVE_SPEED_WATER
                                : Constants::MOVE_SPEED_GROUND;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x = -moveSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x = moveSpeed;
    }
    else
    {
        velocity.x = 0.f;
    }
}

void Player::handleVerticalInput(float dt)
{
    // Jump buffer
    jumpBufferTime = std::max(0.f, jumpBufferTime - dt);

    if (isSwimming())
    {
        if (jumpBufferTime > 0.f)
        {
            velocity.y = -Constants::WATER_JUMP_STRENGTH;
            jumpBufferTime = 0.f;
            waterJumpLock = 0.1f;
        }
        else if (waterJumpLock <= 0.f)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                velocity.y = -Constants::SWIM_SPEED_VERTICAL;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                velocity.y = Constants::SWIM_SPEED_VERTICAL;
            }
            else
            {
                velocity.y = Constants::SINK_SPEED;
            }
        }
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
        if (jumpBufferTime > 0.f &&
            coyoteTime > 0.f)
        {
            velocity.y = -Constants::JUMP_STRENGTH;
            isGrounded = false;
            jumpBufferTime = 0.f;
        }
    }

    waterJumpLock = std::max(0.f, waterJumpLock - dt);
}

void Player::applyEnvironmentForces(float dt)
{
    // Gravity
    if (!isSwimming() && !isGrounded)
    {
        velocity.y += Constants::GRAVITY * dt;
    }
}

bool Player::isSwimming() const
{
    return currentTileType == TileType::WATER;
}
