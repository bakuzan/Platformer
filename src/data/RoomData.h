#pragma once

#include <string>
#include <vector>

#include "data/RoomEntity.h"

struct RoomData
{
    std::vector<std::string> tileGrid;
    std::vector<RoomEntity> entities;
};