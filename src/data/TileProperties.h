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
    float damageAmount;

    static TileProperties makeEmpty()
    {
        return {TileCategory::EMPTY, Solidity::NONE, false, false, false, 0.f};
    }

    static TileProperties createSolidTileProperties(bool breakable = false,
                                                    bool climbable = false,
                                                    float damageAmount = 0.f)
    {
        return createTileProperties(TileCategory::SOLID,
                                    Solidity::BOTH,
                                    breakable, climbable,
                                    damageAmount);
    }

    static TileProperties createWaterTileProperties(float damageAmount = 0.f)
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
                                               float damageAmount = 0.f)
    {
        return {type, solid, breakable, damageAmount > 0, climbable, damageAmount};
    }
};