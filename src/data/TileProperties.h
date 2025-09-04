#pragma once

#include "constants/TileType.h"

struct TileProperties
{
    TileType type;
    bool canMoveThrough;
    bool causesDamage;
    bool isCheckpoint;
};