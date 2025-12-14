#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <vector>

#include "data/RoomEntity.h"
#include "entities/Enemy.h"
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
    void processPlayerAbility(
        std::shared_ptr<Player> player,
        std::vector<std::unique_ptr<Item>> &items,
        RoomEntity entity) const;
    void processEnemy(
        std::vector<std::unique_ptr<Enemy>> &enemies,
        RoomEntity entity) const;

public:
    RoomData() = default;
    RoomData(const std::string filename, float roomTileSize);
    ~RoomData();

    sf::Vector2f getRoomDimensions() const;
    sf::Vector2f getPlayerSpawn(const std::string &spawnKey) const;
    void processRoomEntities(std::shared_ptr<Player> player,
                             std::vector<std::unique_ptr<Item>> &items,
                             std::vector<std::unique_ptr<Enemy>> &enemies) const;
};