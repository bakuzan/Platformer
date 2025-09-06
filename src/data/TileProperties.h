#pragma once

#include "constants/TileType.h"

struct TileProperties
{
    TileType type;
    bool isSolid;
    bool isDamaging;
    bool isCheckpoint;
};