#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#include "data/RoomEntity.h"

struct RoomData
{
    std::vector<std::string> tileGrid;
    std::vector<RoomEntity> entities;

    sf::Vector2f getRoomDimensions(float tileSize) const
    {
        if (tileGrid.size() == 0)
        {
            return sf::Vector2f(0.0f, 0.0f);
        }

        return sf::Vector2f(static_cast<int>(tileGrid[0].size()) * tileSize,
                            static_cast<int>(tileGrid.size()) * tileSize);
    }

    sf::Vector2f getPlayerSpawn(const std::string &spawnKey, float tileSize) const
    {
        for (const auto &e : entities)
        {
            auto it = e.properties.find("spawn");

            if (it != e.properties.end() &&
                it->second == spawnKey)
            {
                return {e.x * tileSize,
                        e.y * tileSize};
            }
        }

        throw std::runtime_error("No spawn found for key: " + spawnKey);
    }
};