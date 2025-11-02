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
#include "SaveMenuState.h"

GameState::GameState(GameData &data, StateManager &manager, sf::RenderWindow &win,
                     SaveData saveData)
    : gameData(data),
      stateManager(manager),
      window(win),
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
    inputManager.bind(Action::DASH, sf::Keyboard::LShift);

    // Setup player object
    auto player = std::make_shared<Player>();
    gameData.setPlayer(player);

    for (const auto &ability : saveData.playerAbilities)
    {
        player->setAbility(ability);
    }

    // Load current room
    loadMap(saveData.room, saveData.spawn);
}

GameState::~GameState()
{
    // Destructor
}

// Publics

void GameState::handleEvent(const sf::Event &event)
{
    handleSystemEvents(event);
    handlePlayerEvents(event);
    uiManager.handleEvent(event);
}

void GameState::handleWindowResize(sf::Vector2u newSize)
{
    camera.handleResize(newSize.x, newSize.y);
    uiManager.handleResize(newSize.x, newSize.y);
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

    // Update player
    bool leftHeld = inputManager.isDown(Action::MOVE_LEFT);
    bool rightHeld = inputManager.isDown(Action::MOVE_RIGHT);
    bool upHeld = inputManager.isDown(Action::MOVE_UP);
    bool downHeld = inputManager.isDown(Action::MOVE_DOWN);

    player->handleHorizontalInput(dt, leftHeld, rightHeld);
    player->handleVerticalInput(dt, upHeld, downHeld);
    player->update(dt, physicsSystem);

    // Check collisions
    checkEntrances(roomData, playerBounds);
    checkSavePoints(roomData, playerBounds);

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
                              {player->getPosition().x + (player->getBounds().width / 2.0f),
                               player->getPosition().y});
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
        sf::FloatRect rect = GameUtils::getRectForRoomEntity(point, tileMap.tileSize);
        sf::RectangleShape safePoint(sf::Vector2f(rect.width, rect.height));
        safePoint.setFillColor(sf::Color(255, 165, 0));
        safePoint.setPosition(rect.left, rect.top);

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
                        const std::string playerSpawnKey)
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
    auto player = gameData.getPlayer();
    auto &items = gameData.getItems();
    roomData.processRoomItems(items, player);

    // TODO do other entities ??

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
        sf::FloatRect rect = GameUtils::getRectForRoomEntity(e, tileMap.tileSize);
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
        sf::FloatRect rect = GameUtils::getRectForRoomEntity(sp, tileMap.tileSize);

        if (rect.intersects(playerBounds))
        {
            canSaveHere = true;
            auto it = sp.properties.find("spawn");
            if (it != sp.properties.end())
            {
                currentSavePoint = it->second;
            }
            break;
        }
    }
}

void GameState::handlePlayerEvents(const sf::Event &event)
{
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

    if (event.type == sf::Event::KeyPressed &&
        inputManager.isPressed(Action::DASH, event.key.code))
    {
        player->onDashPressed();
    }
}

void GameState::handleSystemEvents(const sf::Event &event)
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
            stateManager.pushState(
                std::make_unique<SaveMenuState>(gameData, stateManager, window, currentSavePoint));
        }
    }
}