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
    // Horizontal input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x = -speed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x = speed;
    }
    else
    {
        velocity.x = 0.f;
    }

    // Handle jumping timers
    if (isGrounded)
    {
        coyoteTime = Constants::COYOTE_TIME;
    }
    else
    {
        coyoteTime -= dt;
        if (coyoteTime < 0.f)
        {
            coyoteTime = 0.f;
        }
    }

    jumpBufferTime -= dt;
    if (jumpBufferTime < 0.f)
    {
        jumpBufferTime = 0.f;
    }

    // Jumping action
    if (jumpBufferTime > 0.f &&
        coyoteTime > 0.f)
    {
        velocity.y = -Constants::JUMP_STRENGTH;
        isGrounded = false;
        jumpBufferTime = 0.f;
    }

    // Handle player dropping from platform
    if (dropThroughTimer > 0.f)
    {
        dropThroughTimer -= dt;
        if (dropThroughTimer < 0.f)
        {
            dropThroughTimer = 0.f;
        }
    }

    // Apply gravity
    if (!isGrounded)
    {
        velocity.y += Constants::GRAVITY * dt;
    }

    // Collision resolution
    PhysicsResult res = physics.moveAndCollide(
        getBounds(),
        velocity,
        dt,
        dropThroughTimer > 0.f);

    setPosition(res.position);
    velocity = res.velocity;
    isGrounded = res.grounded;
}

void Player::render(sf::RenderWindow &window) const
{
    window.draw(sprite);
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