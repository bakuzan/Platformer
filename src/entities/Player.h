#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include <set>

#include "constants/PlayerAbility.h"
#include "constants/TileType.h"

class PhysicsSystem;

class Player
{
private:
    sf::Sprite sprite;
    sf::Texture texture;

    sf::Vector2f velocity;

    bool isGrounded = false;
    bool isJumpHeld = false;
    float coyoteTime = 0.f;
    float jumpBufferTime = 0.f;
    float dropThroughTimer = 0.f;
    float waterJumpLock = 0.f;

    TileType currentTileType = TileType::EMPTY;

    std::set<PlayerAbility> abilities;

private:
    bool isSwimming() const;
    void applyEnvironmentForces(float dt);

public:
    Player();
    ~Player();

    // void handleEvent(const sf::Event &event);
    void update(float dt, const PhysicsSystem &physics);
    void render(sf::RenderWindow &window) const;

    void setSpawnPosition(sf::Vector2f pos);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    void setAbility(PlayerAbility ability);
    bool hasAbility(PlayerAbility ability) const;
    std::vector<PlayerAbility> getCurrentAbilties() const;

    void handleHorizontalInput(float dt, bool leftHeld, bool rightHeld);
    void handleVerticalInput(float dt, bool upHeld, bool downHeld);
    void onJumpPressed(bool dropThrough);
    void onJumpReleased();
};

#endif // PLAYER_H
