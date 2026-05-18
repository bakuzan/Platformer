#ifndef GAMEMENUSTATE_H
#define GAMEMENUSTATE_H

#include <SFML/Graphics.hpp>
#include <string>

#include "ui/Button.h"
#include "components/LevelMap.h"
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
    sf::FloatRect pauseMapViewport;

    TileRegistry tileRegistry;
    LevelMap levelMap;
    bool pauseMapReady;

    // Double clicking...
    sf::Clock clickClock;
    sf::Time lastClickTime = sf::Time::Zero;
    const sf::Time doubleClickThreshold = sf::milliseconds(250);

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
