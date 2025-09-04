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
      tileMap(tileRegistry.createTileRegistry()),
      uiManager(&window, data),
      levelIndex(0)
{
    loadMap(gameData.getLevelMap(levelIndex));

    // Set up the view
    view.setSize(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT);
    view.setCenter(view.getCenter());
}

GameState::~GameState()
{
    // Destructor
}

// Publics

void GameState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::Resized)
    {
        // Maintain the height of the view to match the window height
        float aspectRatio = float(window.getSize().x) / float(window.getSize().y);

        view.setSize((Constants::VIEW_HEIGHT * aspectRatio),
                     Constants::VIEW_HEIGHT);
    }

    if (event.type == sf::Event::KeyPressed &&
        InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Escape, sf::Keyboard::P}))
    {
        stateManager.pushState(std::make_unique<GameMenuState>(gameData, stateManager, window));
    }

    uiManager.handleEvent(event);
}

void GameState::update(sf::Time deltaTime)
{
    // TODO restore when implemented
    // if (gameData.audioManager.getSoundStatus(AudioId::AMBIENT) != sf::Sound::Status::Playing)
    // {
    //     gameData.audioManager.playSound(AudioId::AMBIENT, true);
    // }

    float dt = deltaTime.asSeconds();
    (void)dt;

    // UI handling
    uiManager.update();
}

void GameState::render()
{
    // Core gameplay rendering
    window.setView(view);

    tileMap.render(window);

    // UI elements rendering
    uiManager.render();
}

// Private

void GameState::loadMap(const std::string filename)
{
    auto roomData = RoomLoader::loadFromFile(filename);
    tileMap.loadFromRoom(roomData);
    // TODO Load Entities from roomData
}

void GameState::onPlayerDeath()
{
    stateManager.pushState(std::make_unique<GameOverState>(gameData, stateManager, window));
}
