#include "RoomData.h"

RoomData::RoomData(float roomTileSize)
    : tileSize(roomTileSize)
{
}

RoomData::~RoomData()
{
    // Destructor
}

// Publics

sf::Vector2f RoomData::getRoomDimensions() const
{
    if (tileGrid.size() == 0)
    {
        return sf::Vector2f(0.0f, 0.0f);
    }

    return sf::Vector2f(static_cast<int>(tileGrid[0].size()) * tileSize,
                        static_cast<int>(tileGrid.size()) * tileSize);
}

sf::Vector2f RoomData::getPlayerSpawn(const std::string &spawnKey) const
{

    for (const auto &e : entities)
    {
        auto it = e.properties.find("spawn");

        if (it != e.properties.end() &&
            it->second == spawnKey)
        {
            return resolveSpawn(e);
        }
    }

    for (const auto &e : entrances)
    {
        auto it = e.properties.find("spawn");

        if (it != e.properties.end() &&
            it->second == spawnKey)
        {
            return resolveSpawn(e);
        }
    }

    throw std::runtime_error("No spawn found for key: " + spawnKey);
}

std::vector<std::unique_ptr<Item>> RoomData::getItems() const
{
    std::vector<std::unique_ptr<Item>> result;

    for (const auto &e : entities)
    {
        if (e.type == "PowerUp")
        {
            result.push_back(std::make_unique<PowerUp>(e.x * tileSize, e.y * tileSize));
        }
    }

    return result;
}

// Privates

sf::Vector2f RoomData::resolveSpawn(const RoomEntity &e) const
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