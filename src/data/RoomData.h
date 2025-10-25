#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <vector>

#include "data/RoomEntity.h"
#include "entities/Item.h"
#include "entities/PowerUp.h"

class RoomData
{
private:
    float tileSize;

public:
    std::string fileName;
    std::vector<std::string> tileGrid;
    std::vector<RoomEntity> entities;
    std::vector<RoomEntity> entrances;
    std::vector<RoomEntity> savePoints;

private:
    sf::Vector2f resolveSpawn(const RoomEntity &e) const;

public:
    RoomData() = default;
    RoomData(const std::string filename, float roomTileSize);
    ~RoomData();

    sf::Vector2f getRoomDimensions() const;
    sf::Vector2f getPlayerSpawn(const std::string &spawnKey) const;
    void processRoomItems(std::vector<std::unique_ptr<Item>> &items) const;
};