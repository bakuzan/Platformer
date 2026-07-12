#pragma once

#include <array>
#include "utils/EnumUtils.h"

enum class EnemyType
{
    SLUG = 0,
    CHARGER = 1,
    FLIER = 2,
    SWIMMER = 3,
    CRAWLER = 4,
    // Keep COUNT last!
    COUNT,
};

template <>
struct EnumStrings<EnemyType>
{
    static constexpr std::array<const char *, static_cast<size_t>(EnemyType::COUNT)> names = {
        "Slug",
        "Charger",
        "Flier",
        "Swimmer",
        "Crawler"};
};