#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>

#include "entities/Player.h"

class Item
{
private:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    Item(sf::Color colour, float x, float y);
    virtual ~Item() = default;

    void render(sf::RenderWindow &window) const;
    virtual void onPickup(Player &player) = 0;

    sf::Sprite getSprite() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
};

#endif // ITEM_H
