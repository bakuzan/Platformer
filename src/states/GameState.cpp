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
      status(GameStatus::LOADING),
      uiManager(&window, data)
{
    // Setup player object
    auto player = std::make_shared<Player>();
    gameData.setPlayer(player);

    // Load current room
    loadMap("resources/maps/room_01.txt", "door1-2"); // TODO load this from save or whatever
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

    auto roomData = gameData.getRoomData();
    auto player = gameData.getPlayer();
    player->update(dt, physicsSystem);
    checkEntrances(roomData, player->getBounds());

    // UI handling
    uiManager.update();

    // Re-center the view
    sf::Vector2f roomDims = roomData.getRoomDimensions(tileMap.tileSize);

    camera.follow(
        player->getPosition(),
        roomDims.x, roomDims.y);
}

void GameState::render()
{
    if (status == GameStatus::LOADING)
    {
        return;
    }

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
    status = GameStatus::LOADING;

    gameData.setRoomData(RoomLoader::loadFromFile(filename));
    auto roomData = gameData.getRoomData();
    tileMap.loadFromRoom(roomData);

    // Process room entities
    sf::Vector2f spawnPos = roomData.getPlayerSpawn(playerSpawnKey, tileMap.tileSize);
    gameData.getPlayer()->setSpawnPosition(spawnPos);

    // TODO do the entities other than Player

    status = GameStatus::PLAYING;
}

void GameState::onPlayerDeath()
{
    stateManager.pushState(std::make_unique<GameOverState>(gameData, stateManager, window));
}

bool GameState::hasExited(const sf::FloatRect &playerBounds,
                          const sf::FloatRect &entranceRect,
                          const std::string &exitDir)
{
    if (!playerBounds.intersects(entranceRect))
    {
        return false;
    }

    float playerCenterX = playerBounds.left + (playerBounds.width / 2.f);
    float playerCenterY = playerBounds.top + (playerBounds.height / 2.f);

    if (exitDir == "up")
    {
        return playerCenterY < entranceRect.top;
    }
    else if (exitDir == "down")
    {
        return playerCenterY > (entranceRect.top + entranceRect.height);
    }
    else if (exitDir == "left")
    {
        return playerCenterX < entranceRect.left;
    }
    else if (exitDir == "right")
    {
        return playerCenterX > (entranceRect.left + entranceRect.width);
    }

    return false;
}

void GameState::checkEntrances(const RoomData &currentRoom,
                               const sf::FloatRect &playerBounds)
{
    for (const auto &e : currentRoom.entrances)
    {
        sf::FloatRect rect(
            e.x * tileMap.tileSize,
            e.y * tileMap.tileSize,
            std::stoi(e.properties.at("width")) * tileMap.tileSize,
            std::stoi(e.properties.at("height")) * tileMap.tileSize);

        const std::string &exitDir = e.properties.at("exitDir");

        if (hasExited(playerBounds, rect, exitDir))
        {
            loadMap(e.properties.at("target"),
                    e.properties.at("spawn"));
            break;
        }
    }
}
