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

    void prepare(const sf::RenderWindow &window,
                 const sf::FloatRect &viewport,
                 const sf::Vector2f playerPos,
                 const std::vector<std::string> &levelRooms,
                 const std::string &startRoomId);
    void handleZoom(const sf::RenderWindow &window,
                    float wheelDelta);
    void handleMousePress(const sf::Event::MouseButtonEvent &e);
    void handleMouseRelease(const sf::Event::MouseButtonEvent &e);
    void handleMouseMove(const sf::RenderWindow &window);
    void handleDoubleClick();
    void render(sf::RenderWindow &window,
                const sf::FloatRect &viewport);

    bool isReady() const;

private:
    sf::Vector2f loadPlayerLevelPosition(const std::string &startRoomId, const sf::Vector2f &playerPos);
    void recenterOnPlayer();
    std::unordered_map<std::string, RoomData> loadLevelRoomData(const std::vector<std::string> &levelRooms);
    std::unordered_map<std::string, sf::Vector2i> computeRoomOffsets(const std::string &startRoomId);
    void buildPlacedRoomsFromOffsets(const std::unordered_map<std::string, sf::Vector2i> &offsets);
    void buildTileMap();
    void syncRevealedFromGameData();

    void renderRoomOutlines(sf::RenderWindow &window);
    void renderEntities(sf::RenderWindow &window);
    void renderFog(sf::RenderWindow &window);

    void applyClamping();

private:
    TileMap tileMap;
    GameData &gameData;
    sf::View view;
    sf::RectangleShape background;

    sf::Vector2f lastPlayerLevelPos;
    sf::Vector2f defaultViewSize;

    // Zoom
    float zoomLevel = 1.f;
    const float zoomMin = 0.4f;
    const float zoomMax = 2.5f;

    // Dragging
    bool dragging = false;
    sf::Vector2i lastMouse;

    // Clamping
    bool clampCamera = true;
    int minimumRequiredTilesX = 5;
    int minimumRequiredTilesY = 5;

    // Colours
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
