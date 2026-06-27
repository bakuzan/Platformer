#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>

class HealthBar
{
private:
    sf::RectangleShape background;
    sf::RectangleShape foreground;
    sf::Vector2f dimensions;
    float inset;

public:
    HealthBar(sf::Vector2f size = {50.f, 6.f}, float outlineInset = 2.f);

    void update(int health, int maxHealth, sf::Vector2f position);
    void render(sf::RenderWindow &window) const;
};

#endif // HEALTHBAR_H
