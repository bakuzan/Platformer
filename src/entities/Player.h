#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "components/PhysicsSystem.h"

class Player
{
private:
    sf::Sprite sprite;
    sf::Texture texture;

    float speed = 200.f; // pixels per second
    bool grounded = false;
    sf::Vector2f velocity;

public:
    Player();
    ~Player();

    void handleEvent(const sf::Event &event);
    void update(float dt, const PhysicsSystem &physics);
    void render(sf::RenderWindow &window) const;

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
};

#endif // PLAYER_H
