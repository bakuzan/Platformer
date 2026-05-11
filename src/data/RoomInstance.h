#pragma once

#include <SFML/Graphics.hpp>

#include "data/RoomData.h"

struct RoomInstance
{
    RoomData room;
    sf::Vector2i offsetTiles;
};
