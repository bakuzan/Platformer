#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#include "data/RoomEntity.h"

struct RoomData
{
public:
    std::vector<std::string> tileGrid;
    std::vector<RoomEntity> entities;
    std::vector<RoomEntity> entrances;

public:
    sf::Vector2f getRoomDimensions(float tileSize) const
    {
        if (tileGrid.size() == 0)
        {
            return sf::Vector2f(0.0f, 0.0f);
        }

        return sf::Vector2f(static_cast<int>(tileGrid[0].size()) * tileSize,
                            static_cast<int>(tileGrid.size()) * tileSize);
    }

    sf::Vector2f getPlayerSpawn(const std::string &spawnKey,
                                float tileSize) const
    {

        for (const auto &e : entities)
        {
            auto it = e.properties.find("spawn");

            if (it != e.properties.end() &&
                it->second == spawnKey)
            {
                return resolveSpawn(e, tileSize);
            }
        }

        for (const auto &e : entrances)
        {
            auto it = e.properties.find("spawn");

            if (it != e.properties.end() &&
                it->second == spawnKey)
            {
                return resolveSpawn(e, tileSize);
            }
        }

        throw std::runtime_error("No spawn found for key: " + spawnKey);
    }

private:
    sf::Vector2f resolveSpawn(const RoomEntity &e,
                              float tileSize) const
    {
        int sx = e.x;
        int sy = e.y;

        if (auto sxIt = e.properties.find("spawnX"); sxIt != e.properties.end())
        {
            sx = std::stoi(sxIt->second);
        }

        if (auto syIt = e.properties.find("spawnY"); syIt != e.properties.end())
        {
            sy = std::stoi(syIt->second);
        }

        return {sx * tileSize,
                sy * tileSize};
    }
};