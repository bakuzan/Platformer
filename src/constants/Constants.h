#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>

namespace Constants
{
    // Dimensions
    constexpr float VIEW_WIDTH = 960.0f;
    constexpr float VIEW_HEIGHT = 540.0f;

    constexpr float BUTTON_WIDTH = 250.0f;
    constexpr float BUTTON_HEIGHT = 50.0f;

    constexpr float TILE_SIZE = 32.0f;

    // Movement
    constexpr float MOVE_SPEED_GROUND = 200.f;
    constexpr float MOVE_SPEED_WATER = 120.f;
    constexpr float SWIM_SPEED_VERTICAL = 100.f;
    constexpr float WATER_JUMP_STRENGTH = 200.f;
    constexpr float SINK_SPEED = 45.f;
    constexpr float GRAVITY = 980.f;
    constexpr float JUMP_STRENGTH = 450.f;

    constexpr float COYOTE_TIME = 0.1f;
    constexpr float JUMP_BUFFER_TIME = 0.1f;
    constexpr float INVINCIBLE_TIME = 1.25f;
    constexpr float MIN_FLASH_INTERVAL = 0.03f;
    constexpr float MAX_FLASH_INTERVAL = 0.25f;

    constexpr float DASH_DURATION = 0.25f;
    constexpr float DASH_SPEED_GROUND = 600.f;
    constexpr float DASH_SPEED_WATER = 360.f;
    constexpr float DASH_COOLDOWN = 1.25f;

    constexpr float SMASH_SPEED = 1200.f;
    constexpr float SMASH_COOLDOWN = 1.f;
    constexpr float SMASH_BUFFER_TIME = 0.25f;

    constexpr float WALL_JUMP_PUSH = 45.f;

    // Entities
    const std::string ENTITY_ENEMY = "Enemy";
    const std::string ENTITY_PLAYER_ABILITY = "PlayerAbility";

    // Colours
    const sf::Color uiBackgroundColour(0, 45, 90, 255);
    const sf::Color savepointColour(sf::Color::White);
    const sf::Color playerColour(sf::Color::Yellow);
    const sf::Color playerAbilityColour(sf::Color::Magenta);
};

#endif // CONSTANTS_H
