#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
private:
    sf::Sprite sprite;
    sf::Texture texture;

    float speed = 200.f; // pixels per second

public:
    Player();
    ~Player();

    void handleEvent(const sf::Event &event);
    void update(float dt);
    void render(sf::RenderWindow &window) const;

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
};

#endif // PLAYER_H
