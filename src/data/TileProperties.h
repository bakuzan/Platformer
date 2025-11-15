#pragma once

#include "constants/TileCategory.h"
#include "constants/Solidity.h"

struct TileProperties
{
    TileCategory type;
    Solidity solidity;
    bool isBreakable;

    static TileProperties makeEmpty()
    {
        return {TileCategory::EMPTY, Solidity::NONE, false};
    }

    static TileProperties createTileProperties(TileCategory type,
                                               Solidity solid, bool breakable)
    {
        return {type, solid, breakable};
    }
};