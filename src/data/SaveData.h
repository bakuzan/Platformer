#pragma once

#include "constants/PlayerAbility.h"

struct SaveData
{
    std::string timestamp;
    std::string room;
    std::string spawn;
    std::vector<PlayerAbility> playerAbilities;

    static SaveData makeDefault()
    {
        SaveData d;
        d.timestamp = "";
        d.room = "resources/maps/room_01.txt";
        d.spawn = "default";
        d.playerAbilities = {};
        return d;
    }
};