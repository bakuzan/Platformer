#pragma once

#include "constants/TileType.h"
#include "constants/Solidity.h"

struct TileProperties
{
    TileType type;
    Solidity solidity;
    bool isDamaging;
    bool isCheckpoint;

    static TileProperties createTileProperties(TileType type,
                                               Solidity solid, bool damaging, bool checkpoint)
    {
        return {type, solid, damaging, checkpoint};
    }
};