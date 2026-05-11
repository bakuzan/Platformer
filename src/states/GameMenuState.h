#ifndef GAMEMENUSTATE_H
#define GAMEMENUSTATE_H

#include <SFML/Graphics.hpp>
#include <string>

#include "ui/Button.h"
#include "components/TileMap.h"
#include "components/TileRegistry.h"
#include "core/GameData.h"
#include "core/State.h"
#include "core/StateManager.h"
#include "data/RoomInstance.h"
#include "entities/Player.h"

class GameMenuState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;
    sf::View pauseView;

    TileRegistry tileRegistry;
    TileMap pauseTileMap;
    std::vector<RoomInstance> pausePlacedRooms;
    sf::Vector2i pauseLevelSizeTiles{0, 0};
    bool pauseMapReady = false;

    sf::RectangleShape background;
    sf::Text pauseText;

    float buttonSpacing;
    std::vector<Button> buttons;
    int selectedButtonIndex = 0;

private:
    void addButton(std::string id,
                   std::string label,
                   std::function<void()> callback);
    void updateMenuItemPositions();
    void renderAbilitiesPanel(const Player &player);

    // Map
    void assemblePlacedRooms(const std::vector<std::string> &levelRooms,
                             const std::string &startRoomId);
    std::unordered_map<std::string, RoomData> loadLevelRoomData(
        const std::vector<std::string> &levelRooms);
    std::unordered_map<std::string, sf::Vector2i> computeRoomOffsets(
        const std::unordered_map<std::string, RoomData> &roomDataMap,
        const std::string &startRoomId);
    void buildPlacedRoomsFromOffsets(
        const std::unordered_map<std::string, RoomData> &roomDataMap,
        const std::unordered_map<std::string, sf::Vector2i> &offsets);
    void buildPauseTileMap();
    void preparePauseMap();

public:
    GameMenuState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~GameMenuState();

    void handleEvent(const sf::Event &event) override;
    void handleWindowResize(sf::Vector2u newSize) override;
    void update(sf::Time deltaTime) override;
    void render() override;
};

#endif // GAMEMENUSTATE_H
