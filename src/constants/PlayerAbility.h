#pragma once

#include <array>
#include "utils/EnumUtils.h"

enum class PlayerAbility
{
    DOUBLE_JUMP = 0,
    DASH = 1,
    SMASH = 2,
    WALL_CLIMB = 3,
    // Keep COUNT last!
    COUNT,
};

template <>
struct EnumStrings<PlayerAbility>
{
    static constexpr std::array<const char *, static_cast<size_t>(PlayerAbility::COUNT)> names = {
        "AIR JUMP",
        "DASH",
        "SMASH",
        "CLIMB"};
};
