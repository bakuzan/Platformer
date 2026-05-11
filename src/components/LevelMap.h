#ifndef LEVELMAP_H
#define LEVELMAP_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

#include "components/TileMap.h"
#include "components/RoomLoader.h"
#include "core/GameData.h"
#include "data/RoomData.h"
#include "data/RoomInstance.h"

class LevelMap
{
public:
    LevelMap(const std::unordered_map<char, TileDefinition> &registry, GameData &gameData);
    ~LevelMap();

    void prepare(const std::vector<std::string> &levelRooms,
                 const std::string &startRoomId);

    void render(sf::RenderWindow &window,
                const sf::FloatRect &viewport);

    bool isReady() const;
    sf::Vector2i getLevelSizeTiles() const;
    const std::vector<RoomInstance> &getPlacedRooms() const;

private:
    std::unordered_map<std::string, RoomData> loadLevelRoomData(const std::vector<std::string> &levelRooms);
    std::unordered_map<std::string, sf::Vector2i> computeRoomOffsets(
        const std::unordered_map<std::string, RoomData> &roomDataMap,
        const std::string &startRoomId);
    void buildPlacedRoomsFromOffsets(
        const std::unordered_map<std::string, RoomData> &roomDataMap,
        const std::unordered_map<std::string, sf::Vector2i> &offsets);
    void buildTileMap();

private:
    TileMap tileMap;
    GameData &gameData;

    std::vector<RoomInstance> placedRooms;
    sf::Vector2i levelSizeTiles{0, 0};

    bool ready = false;
};

#endif // LEVELMAP_H
