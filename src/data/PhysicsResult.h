#pragma once

#include <SFML/Graphics.hpp>

#include "constants/TileType.h"

struct PhysicsResult
{
    TileType tileType;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool grounded;
};