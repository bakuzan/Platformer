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
    std::unordered_map<std::string, std::vector<std::vector<bool>>> revealedTiles;

    static SaveData makeDefault()
    {
        SaveData d;
        d.timestamp = "";
        d.room = "room_01";
        d.spawn = "default";
        d.playerAbilities = {};
        d.destroyedTiles = {};
        d.revealedTiles = {};
        return d;
    }

    static SaveData makeSandbox()
    {
        SaveData d = SaveData::makeDefault();
        d.room = "test";
        EnumUtils::forEachEnum<PlayerAbility>(
            [&](PlayerAbility a)
            { d.playerAbilities.push_back(a); });

        return d;
    }
};