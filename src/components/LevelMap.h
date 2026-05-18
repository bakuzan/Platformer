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
    void handleZoom(const sf::RenderWindow &window,
                    float wheelDelta);
    void render(sf::RenderWindow &window,
                const sf::FloatRect &viewport);

    bool isReady() const;

private:
    std::unordered_map<std::string, RoomData> loadLevelRoomData(const std::vector<std::string> &levelRooms);
    std::unordered_map<std::string, sf::Vector2i> computeRoomOffsets(const std::string &startRoomId);
    void buildPlacedRoomsFromOffsets(const std::unordered_map<std::string, sf::Vector2i> &offsets);
    void buildTileMap();
    void syncRevealedFromGameData();

    void renderEntities(sf::RenderWindow &window);
    void renderFog(sf::RenderWindow &window);

private:
    TileMap tileMap;
    GameData &gameData;
    sf::View view;
    sf::RectangleShape background;

    float zoomLevel = 1.f;
    const float zoomMin = 0.25f;
    const float zoomMax = 4.f;

    sf::Color blackoutColour;
    sf::Color savepointColour;
    sf::Color itemColour;

    std::vector<RoomInstance> placedRooms;
    sf::Vector2i levelSizeTiles{0, 0};
    std::vector<std::vector<bool>> revealedGrid;
    std::unordered_map<std::string, RoomData> roomDataMap;

    bool ready = false;
};

#endif // LEVELMAP_H
