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
    bool touchingWallLeft;
    bool touchingWallRight;

    static PhysicsResult create()
    {
        return {TileProperties::makeEmpty(),
                {0, 0},
                {0.f, 0.f},
                {0.f, 0.f},
                false,
                false,
                false};
    }
};