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
    std::string locationName;
    std::string spawn;
    std::vector<PlayerAbility> playerAbilities;
    std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>> destroyedTiles;
    std::unordered_map<std::string, std::vector<std::vector<bool>>> revealedTiles;

    static SaveData makeDefault()
    {
        SaveData d;
        d.timestamp = "";
        d.room = "room_003";
        d.locationName = "";
        d.spawn = "default";
        d.playerAbilities = {};
        d.destroyedTiles = {};
        d.revealedTiles = {};
        return d;
    }

    static SaveData makeSandbox()
    {
        SaveData d = SaveData::makeDefault();
        d.room = "room_034"; // TODO put this to actual sanbox when finished doing levels
        d.spawn = "door5-7";
        EnumUtils::forEachEnum<PlayerAbility>(
            [&](PlayerAbility a)
            { d.playerAbilities.push_back(a); });

        return d;
    }
};