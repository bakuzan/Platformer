#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

#include "core/GameData.h"
#include "core/State.h"
#include "core/StateManager.h"
#include "core/InputManager.h"

#include "components/Camera.h"
#include "components/PhysicsSystem.h"
#include "components/TileMap.h"
#include "components/TileRegistry.h"
#include "components/UIManager.h"

#include "constants/GameStatus.h"

#include "data/RoomData.h"

class GameState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;

    Camera camera;
    TileMap tileMap;
    TileRegistry tileRegistry;
    PhysicsSystem physicsSystem;
    GameStatus status;

    UIManager uiManager;
    InputManager inputManager;

    bool canSaveHere;
    sf::FloatRect currentSaveRect;

private:
    void loadMap(const std::string filename,
                 const std::string &playerSpawnKey);
    void onPlayerDeath();
    bool hasExited(const sf::FloatRect &playerBounds,
                   const sf::FloatRect &entranceRect,
                   const std::string &exitDir);
    void checkEntrances(const RoomData &currentRoom,
                        const sf::FloatRect &playerBounds);
    void checkSavePoints(const RoomData &currentRoom,
                         const sf::FloatRect &playerBounds);

public:
    GameState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~GameState();

    void handleEvent(const sf::Event &event) override;
    void handleWindowResize(sf::Vector2u newSize) override;
    void update(sf::Time deltaTime) override;
    void render() override;
};

#endif // GAMESTATE_H
