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
      uiManager(&window, data),
      canSaveHere(false)
{
    // Setup Input Manager
    inputManager.bind(Action::PAUSE, sf::Keyboard::Escape);
    inputManager.bind(Action::SAVE, sf::Keyboard::Enter);

    inputManager.bind(Action::MOVE_LEFT, sf::Keyboard::A);
    inputManager.bind(Action::MOVE_RIGHT, sf::Keyboard::D);
    inputManager.bind(Action::MOVE_UP, sf::Keyboard::W);
    inputManager.bind(Action::MOVE_DOWN, sf::Keyboard::S);
    inputManager.bind(Action::JUMP, sf::Keyboard::Space);
    inputManager.bind(Action::DROP_DOWN, sf::Keyboard::S);

    // Setup player object
    auto player = std::make_shared<Player>();
    gameData.setPlayer(player);

    // Load current room
    loadMap("resources/maps/room_03.txt", "door2-3"); // TODO load this from save or whatever
}

GameState::~GameState()
{
    // Destructor
}

// Publics

void GameState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed &&
        inputManager.isPressed(Action::PAUSE, event.key.code))
    {
        stateManager.pushState(std::make_unique<GameMenuState>(gameData, stateManager, window));
    }

    if (event.type == sf::Event::KeyPressed &&
        inputManager.isPressed(Action::SAVE, event.key.code))
    {
        if (canSaveHere)
        {
            // TODO Save the game!
            // saveGame(player, currentRoom);
        }
    }

    auto player = gameData.getPlayer();

    if (event.type == sf::Event::KeyPressed &&
        inputManager.isPressed(Action::JUMP, event.key.code))
    {
        bool drop = inputManager.isDown(Action::DROP_DOWN);
        player->onJumpPressed(drop);
    }
    else if (event.type == sf::Event::KeyReleased &&
             inputManager.isPressed(Action::JUMP, event.key.code))
    {
        player->onJumpReleased();
    }

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

    auto &roomData = gameData.getRoomData();
    auto player = gameData.getPlayer();
    sf::FloatRect playerBounds = player->getBounds();

    bool leftHeld = inputManager.isDown(Action::MOVE_LEFT);
    bool rightHeld = inputManager.isDown(Action::MOVE_RIGHT);
    bool upHeld = inputManager.isDown(Action::MOVE_UP);
    bool downHeld = inputManager.isDown(Action::MOVE_DOWN);

    player->handleHorizontalInput(dt, leftHeld, rightHeld);
    player->handleVerticalInput(dt, upHeld, downHeld);
    player->update(dt, physicsSystem);
    checkEntrances(roomData, playerBounds);

    // Check Items
    auto &items = gameData.getItems();
    for (auto itemIt = items.begin(); itemIt != items.end();)
    {
        auto &item = **itemIt;

        if (item.getBounds().intersects(playerBounds))
        {
            item.onPickup(*player);
            itemIt = items.erase(itemIt);
            continue;
        }
        else
        {
            ++itemIt;
        }
    }

    if (canSaveHere)
    {
        uiManager.showTooltip("Press " + inputManager.getKeyName(Action::SAVE) + " to Save",
                              {player->getPosition().x, player->getPosition().y - 20});
    }
    else
    {
        uiManager.clearTooltip();
    }

    // UI handling
    uiManager.update();

    // Re-center the view
    sf::Vector2f roomDims = roomData.getRoomDimensions();

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
    auto &roomData = gameData.getRoomData();
    for (const auto &point : roomData.savePoints)
    {
        float x = static_cast<float>(point.x);
        float y = static_cast<float>(point.y);
        float w = std::stoi(point.properties.at("width")) * tileMap.tileSize;
        float h = std::stoi(point.properties.at("height")) * tileMap.tileSize;

        sf::RectangleShape safePoint(sf::Vector2f(w, h));
        safePoint.setFillColor(sf::Color(255, 165, 0));
        safePoint.setPosition(x, y);

        window.draw(safePoint);
    }

    auto player = gameData.getPlayer();
    player->render(window);

    auto &items = gameData.getItems();
    for (const auto &item : items)
    {
        item->render(window);
    }

    // UI elements rendering
    uiManager.render();
}

// Private

void GameState::loadMap(const std::string filename,
                        const std::string &playerSpawnKey)
{
    status = GameStatus::LOADING;

    gameData.resetLevel();

    // Set room
    gameData.setRoomData(RoomLoader::loadFromFile(filename, tileMap.tileSize));
    auto roomData = gameData.getRoomData();
    tileMap.loadFromRoom(roomData);

    // Set player
    sf::Vector2f spawnPos = roomData.getPlayerSpawn(playerSpawnKey);
    gameData.getPlayer()->setSpawnPosition(spawnPos);

    // Process room entities
    auto &items = gameData.getItems();
    roomData.processRoomItems(items);

    // TODO do other entities

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

void GameState::checkSavePoints(const RoomData &currentRoom,
                                const sf::FloatRect &playerBounds)
{
    canSaveHere = false;

    for (const auto &sp : currentRoom.savePoints)
    {
        sf::FloatRect rect(
            sp.x * tileMap.tileSize,
            sp.y * tileMap.tileSize,
            std::stoi(sp.properties.at("width")) * tileMap.tileSize,
            std::stoi(sp.properties.at("height")) * tileMap.tileSize);

        if (rect.intersects(playerBounds))
        {
            canSaveHere = true;
            currentSaveRect = rect;
            break;
        }
    }
}
