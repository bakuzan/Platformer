#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct WorldData
{
    struct LevelInfo
    {
        std::string id;
        std::string name;
        std::vector<std::string> rooms;
    };

    std::unordered_map<std::string, LevelInfo> levels;        // id -> LevelInfo
    std::unordered_map<std::string, std::string> roomToLevel; // room -> level id
};
