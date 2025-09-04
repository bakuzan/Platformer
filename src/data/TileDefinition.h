#pragma once

#include <SFML/Graphics.hpp>

#include "data/TileProperties.h"

struct TileDefinition
{
    sf::Color color;
    TileProperties properties;
};
