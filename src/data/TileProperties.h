#pragma once

#include "constants/TileType.h"
#include "constants/Solidity.h"

struct TileProperties
{
    TileType type;
    Solidity solidity;
    bool isBreakable;

    static TileProperties makeEmpty()
    {
        return {TileType::EMPTY, Solidity::NONE, false};
    }

    static TileProperties createTileProperties(TileType type,
                                               Solidity solid, bool breakable)
    {
        return {type, solid, breakable};
    }
};