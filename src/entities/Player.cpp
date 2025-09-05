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
    (void)event;
    // Handle input events if needed (jump, shoot, etc.)
}

void Player::update(float dt)
{
    // Movement
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movement.y -= speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement.y += speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement.x -= speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x += speed * dt;
    }

    sprite.move(movement);
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