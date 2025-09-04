#pragma once

#include <string>
#include <unordered_map>

struct RoomEntity
{
    std::string type;
    int x, y;
    std::unordered_map<std::string, std::string> properties;
};