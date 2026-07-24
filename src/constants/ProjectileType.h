#pragma once

#include <array>
#include "utils/EnumUtils.h"

enum class ProjectileType
{
    NONE = 0,
    STANDARD = 1,
    HEAVY_SHOT = 2,
    // Keep COUNT last!
    COUNT,
};

template <>
struct EnumStrings<ProjectileType>
{
    static constexpr std::array<const char *, static_cast<size_t>(ProjectileType::COUNT)> names = {
        "NONE",
        "STANDARD",
        "HEAVY SHOT"};
};