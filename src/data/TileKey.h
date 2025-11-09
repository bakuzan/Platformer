#pragma once

struct TileKey
{
    int x;
    int y;

    bool operator==(const TileKey &other) const
    {
        return x == other.x &&
               y == other.y;
    }
};

struct TileKeyHash
{
    std::size_t operator()(const TileKey &k) const noexcept
    {
        return (std::hash<int>()(k.x) << 1) ^ std::hash<int>()(k.y);
    }
};