#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <unordered_map>

#include "core/GameData.h"
#include "core/State.h"
#include "core/StateManager.h"
#include "core/InputManager.h"

#include "components/Camera.h"
#include "components/ProjectileRegistry.h"
#include "components/PhysicsSystem.h"
#include "components/TileMap.h"
#include "components/TileRegistry.h"
#include "components/UIManager.h"

#include "constants/GameStatus.h"
#include "constants/ProjectileType.h"

#include "data/ProjectileConfig.h"
#include "data/RoomData.h"
#include "data/SaveData.h"

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

    ProjectileRegistry projectileRegistryCreator;
    std::unordered_map<ProjectileType, ProjectileConfig> projectileRegistry;

    bool canSaveHere;
    std::string currentSavePoint;

private:
    void loadSaveState(SaveData &saveData,
                       std::shared_ptr<Player> &player);
    void loadMap(const std::string mapRoomId,
                 const std::string playerSpawnKey,
                 std::optional<sf::Vector2i> passingOffset);
    void onPlayerDeath();

    bool hasExited(const sf::FloatRect &playerBounds,
                   const sf::FloatRect &entranceRect,
                   const std::string &exitDir);
    bool checkEntrances(const RoomData &currentRoom,
                        const sf::FloatRect &prevBounds,
                        const sf::FloatRect &newBounds);
    std::optional<sf::Vector2i> computePassingOffset(const RoomEntity &e,
                                                     const sf::FloatRect &playerBounds,
                                                     float tileSize) const;
    void checkSavePoints(const RoomData &currentRoom,
                         const sf::FloatRect &playerBounds);

    void handlePlayerEvents(const sf::Event &event);
    void handlePlayerShooting(float dt, const sf::Vector2f &mouseWorldPos);
    void handleSystemEvents(const sf::Event &event);

    void applyEntranceClearance(const RoomData &currentRoom,
                                const std::string &playerSpawnKey);

    void processTileDestruction(const std::string mapRoomId, sf::Vector2i &p);

    void updateEnemies(float dt, std::shared_ptr<Player> &player);
    void updateProjectiles(float dt);
    void updateItems(float dt, std::shared_ptr<Player> &player);
    void revealTileOnMaps(sf::Vector2f &playerPos, const RoomData &roomData);

public:
    GameState(GameData &data, StateManager &manager, sf::RenderWindow &win,
              SaveData saveData);
    ~GameState();

    void handleEvent(const sf::Event &event) override;
    void handleWindowResize(sf::Vector2u newSize) override;
    void update(sf::Time deltaTime) override;
    void render() override;
};

#endif // GAMESTATE_H
