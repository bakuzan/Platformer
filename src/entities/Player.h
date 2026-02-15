#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include <set>

#include "constants/PlayerAbility.h"
#include "constants/PlayerState.h"
#include "constants/TileCategory.h"
#include "data/EntityCapabilities.h"
#include "data/PhysicsResult.h"

class PhysicsSystem;

class Player
{
private:
    sf::Sprite sprite;
    sf::Texture texture;

    sf::FloatRect previousBounds;
    sf::Vector2f velocity;
    bool facingRight = false;

    // Jump state
    bool isGrounded = false;
    bool isJumpHeld = false;
    int jumpsUsed = 0;
    int maxJumps = 2;
    float coyoteTime = 0.f;
    float jumpBufferTime = 0.f;
    float dropThroughTimer = 0.f;
    float waterJumpLock = 0.f;
    // Dash state
    bool isDashing = false;
    float dashTime = 0.f;
    float dashCooldown = 0.f;
    // Smash move
    bool isSmashing = false;
    float smashCooldown = 0.f;
    // Wall climb
    bool isWallSticking = false;
    bool touchingWallLeft = false;
    bool touchingWallRight = false;

    TileCategory currentTileType = TileCategory::EMPTY;
    std::set<PlayerAbility> abilities;

    // Health
    int maxHealth = 100;
    int health = 100;

    float invincibleTime = 0.f;
    float flashAccumulator = 0.f;

private:
    bool isSwimming() const;
    bool isInvincible() const;

    void applyEnvironmentForces(float dt);

    void updateHealth(int update);

    void updateInvincibilityFeedback(float dt);

public:
    Player(float size);
    ~Player();

    // void handleEvent(const sf::Event &event);
    void update(float dt);
    void render(sf::RenderWindow &window) const;

    void applyPhysicsResult(PhysicsResult &res);
    void setSpawnPosition(sf::Vector2f pos);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
    sf::FloatRect getPreviousBounds() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getVelocity() const;
    bool isTangible() const;

    EntityCapabilities getCapabilities() const;
    void setAbility(PlayerAbility ability);
    bool hasAbility(PlayerAbility ability) const;
    std::vector<PlayerAbility> getCurrentAbilties() const;
    PlayerState getPlayerState() const;

    int getMaxHealth() const;
    int getHealth() const;
    void healDamage(int heal);
    void takeDamage(int damage);
    bool isDead() const;

    void handleHorizontalInput(float dt, bool leftHeld, bool rightHeld);
    void handleVerticalInput(float dt, bool upHeld, bool downHeld);
    void onJumpPressed(bool dropThrough);
    void onJumpReleased();
    void onDashPressed();
    void onSmashPressed();
};

#endif // PLAYER_H
