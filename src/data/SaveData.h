#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "constants/PlayerAbility.h"
#include "data/TileKey.h"
#include "utils/EnumUtils.h"

struct SaveData
{
    std::string timestamp;
    std::string room;
    std::string spawn;
    std::vector<PlayerAbility> playerAbilities;
    std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>> destroyedTiles;

    static SaveData makeDefault()
    {
        SaveData d;
        d.timestamp = "";
        d.room = "resources/maps/room_01.txt";
        d.spawn = "default";
        d.playerAbilities = {};
        d.destroyedTiles = {};
        return d;
    }

    static SaveData makeSandbox()
    {
        SaveData d = SaveData::makeDefault();
        d.room = "resources/maps/test.txt";
        EnumUtils::forEachEnum<PlayerAbility>(
            [&](PlayerAbility a)
            { d.playerAbilities.push_back(a); });

        return d;
    }
};