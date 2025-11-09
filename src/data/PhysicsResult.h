#pragma once

#include <SFML/Graphics.hpp>

#include "data/TileProperties.h"

struct PhysicsResult
{
    TileProperties tileProps;
    sf::Vector2i tilePoint;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool grounded;
};