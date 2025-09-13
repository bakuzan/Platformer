#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "constants/TileType.h"
#include "components/PhysicsSystem.h"

class Player
{
private:
    sf::Sprite sprite;
    sf::Texture texture;

    sf::Vector2f velocity;

    bool isGrounded = false;
    bool isJumpHeld = true;
    float coyoteTime = 0.f;
    float jumpBufferTime = 0.f;
    float dropThroughTimer = 0.f;

    TileType currentTileType = TileType::EMPTY;

private:
    bool isSwimming() const;

    void handleHorizontalInput(float dt);
    void handleVerticalInput(float dt);
    void applyEnvironmentForces(float dt);

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
