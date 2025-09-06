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
        if (grounded)
        {
            velocity.y = -Constants::JUMP_STRENGTH;
            grounded = false;
        }
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

    // Apply gravity
    if (!grounded)
    {
        velocity.y += Constants::GRAVITY * dt;
    }

    // Collision resolution
    PhysicsResult res = physics.moveAndCollide(getBounds(), velocity, dt);

    setPosition(res.position);
    velocity = res.velocity;
    grounded = res.grounded;
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