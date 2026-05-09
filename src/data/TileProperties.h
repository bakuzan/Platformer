#pragma once

#include "constants/TileCategory.h"
#include "constants/Solidity.h"

struct TileProperties
{
    TileCategory type;
    Solidity solidity;
    bool isBreakable;
    bool isDamaging;
    bool isClimbable;

    static TileProperties makeEmpty()
    {
        return {TileCategory::EMPTY, Solidity::NONE, false, false, false};
    }

    static TileProperties createSolidTileProperties(bool breakable = false,
                                                    bool damaging = false,
                                                    bool climbable = false)
    {
        return createTileProperties(TileCategory::SOLID,
                                    Solidity::BOTH,
                                    breakable, damaging, climbable);
    }

    static TileProperties createWaterTileProperties(bool damaging = false)
    {
        return createTileProperties(TileCategory::WATER,
                                    Solidity::NONE,
                                    false, damaging, false);
    }

    static TileProperties createTileProperties(TileCategory type,
                                               Solidity solid,
                                               bool breakable = false,
                                               bool damaging = false,
                                               bool climbable = false)
    {
        return {type, solid, breakable, damaging, climbable};
    }
};