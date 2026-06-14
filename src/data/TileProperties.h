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
    int damageAmount;

    static TileProperties makeEmpty()
    {
        return {TileCategory::EMPTY, Solidity::NONE, false, false, false, 0};
    }

    static TileProperties createSolidTileProperties(bool breakable = false,
                                                    bool climbable = false,
                                                    int damageAmount = 0)
    {
        return createTileProperties(TileCategory::SOLID,
                                    Solidity::BOTH,
                                    breakable, climbable,
                                    damageAmount);
    }

    static TileProperties createWaterTileProperties(int damageAmount = 0)
    {
        return createTileProperties(TileCategory::WATER,
                                    Solidity::NONE,
                                    false, false,
                                    damageAmount);
    }

    static TileProperties createTileProperties(TileCategory type,
                                               Solidity solid,
                                               bool breakable = false,
                                               bool climbable = false,
                                               int damageAmount = 0)
    {
        return {type, solid, breakable, damageAmount > 0, climbable, damageAmount};
    }
};