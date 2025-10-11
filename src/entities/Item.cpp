#include "Item.h"

Item::Item(sf::Color colour, float x, float y)
{
    // TODO Implement the real texture code!
    sf::Image img;
    img.create(32, 32, colour);
    texture.loadFromImage(img);
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

Item::~Item()
{
    // Destructor
}

// Publics

void Item::render(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

sf::Vector2f Item::getPosition() const
{
    return sprite.getPosition();
}

sf::FloatRect Item::getBounds() const
{
    return sprite.getGlobalBounds();
}
