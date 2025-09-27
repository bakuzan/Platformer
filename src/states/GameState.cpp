#include <format>
#include <iostream>

#include "utils/GameUtils.h"
#include "utils/InputUtils.h"
#include "utils/CollisionUtils.h"
#include "components/SettingsManager.h"
#include "constants/AudioId.h"
#include "constants/Constants.h"
#include "data/GameOverStateConfig.h"
#include "components/RoomLoader.h"

#include "GameState.h"
#include "GameMenuState.h"
#include "GameOverState.h"

GameState::GameState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : gameData(data),
      stateManager(manager),
      window(window),
      camera(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT),
      tileMap(tileRegistry.createTileRegistry()),
      physicsSystem(tileMap),
      uiManager(&window, data),
      levelIndex(0)
{
    // Setup player object
    auto player = std::make_shared<Player>();
    gameData.setPlayer(player);

    // Load current room
    loadMap(gameData.getLevelMap(levelIndex),
            "default");
}

GameState::~GameState()
{
    // Destructor
}

// Publics

void GameState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed &&
        InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Escape, sf::Keyboard::P}))
    {
        stateManager.pushState(std::make_unique<GameMenuState>(gameData, stateManager, window));
    }

    auto player = gameData.getPlayer();
    player->handleEvent(event);

    uiManager.handleEvent(event);
}

void GameState::handleWindowResize(sf::Vector2u newSize)
{
    camera.handleResize(newSize.x, newSize.y);
}

void GameState::update(sf::Time deltaTime)
{
    // TODO restore when implemented
    // if (gameData.audioManager.getSoundStatus(AudioId::AMBIENT) != sf::Sound::Status::Playing)
    // {
    //     gameData.audioManager.playSound(AudioId::AMBIENT, true);
    // }

    float dt = deltaTime.asSeconds();

    auto player = gameData.getPlayer();
    player->update(dt, physicsSystem);

    // UI handling
    uiManager.update();

    // Re-center the view
    sf::Vector2f roomDims = gameData.getRoomData().getRoomDimensions(tileMap.tileSize);

    camera.follow(
        player->getPosition(),
        roomDims.x, roomDims.y);
}

void GameState::render()
{
    // Set view and render background
    camera.apply(window);
    tileMap.render(window);

    // Render entities
    auto player = gameData.getPlayer();
    player->render(window);

    // UI elements rendering
    uiManager.render();
}

// Private

void GameState::loadMap(const std::string filename,
                        const std::string &playerSpawnKey)
{
    gameData.setRoomData(RoomLoader::loadFromFile(filename));
    auto roomData = gameData.getRoomData();
    tileMap.loadFromRoom(roomData);

    // Process room entities
    sf::Vector2f spawnPos = roomData.getPlayerSpawn(playerSpawnKey, tileMap.tileSize);
    gameData.getPlayer()->setPosition(spawnPos);

    // TODO do the entities other than Player
}

void GameState::onPlayerDeath()
{
    stateManager.pushState(std::make_unique<GameOverState>(gameData, stateManager, window));
}
