#include "constants/Constants.h"
#include "data/SaveData.h"
#include "utils/InputUtils.h"
#include "GameState.h"
#include "LoadMenuState.h"
#include "MainMenuState.h"
#include "SettingsState.h"

MainMenuState::MainMenuState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win)
{
    buttonSpacing = Constants::BUTTON_HEIGHT + 10.f;
    sf::Vector2f center(menuView.getCenter());
    window.setView(menuView);

    // Setup title
    gameTitle.setFont(gameData.gameFont);
    gameTitle.setString("Platformer");
    gameTitle.setCharacterSize(48);
    gameTitle.setFillColor(sf::Color::White);

    // Add buttons
    float buttonXPos = center.x - 100.f;
    buttons.emplace_back("New", gameData.gameFont, "New Game",
                         sf::Vector2f(buttonXPos, center.y - (buttonSpacing * 2.0f)),
                         [this]()
                         { gameData.reset();
                            stateManager.changeState(std::make_unique<GameState>(gameData, stateManager, window, SaveData::makeDefault())); });
    buttons.emplace_back("Load", gameData.gameFont, "Load Game",
                         sf::Vector2f(buttonXPos, center.y - buttonSpacing),
                         [this]()
                         { stateManager.pushState(std::make_unique<LoadMenuState>(gameData, stateManager, window)); });
    buttons.emplace_back("Settings", gameData.gameFont, "Settings",
                         sf::Vector2f(buttonXPos, center.y + buttonSpacing),
                         [this]()
                         { stateManager.changeState(std::make_unique<SettingsState>(gameData, stateManager, window)); });
    buttons.emplace_back("Exit", gameData.gameFont, "Exit",
                         sf::Vector2f(buttonXPos, center.y + (buttonSpacing * 2.0f)),
                         [this]()
                         { window.close(); });

    // To ensure positioning is updated relative to window resizing
    updateMenuItemPositions();
}

MainMenuState::~MainMenuState()
{
    // Destructor
}

// Publics

void MainMenuState::handleEvent(const sf::Event &event)
{
    InputUtils::handleButtonEvent(event, buttons, window, selectedButtonIndex);
}

void MainMenuState::handleWindowResize(sf::Vector2u newSize)
{
    (void)newSize;
    updateMenuItemPositions();
}

void MainMenuState::update(sf::Time deltaTime)
{
    (void)deltaTime;

    gameData.audioManager.cleanupSounds();

    // if (gameData.audioManager.getSoundStatus(AudioId::AMBIENT) == sf::Sound::Status::Playing)
    // {
    //     gameData.audioManager.stopSound(AudioId::AMBIENT);
    // }
}

void MainMenuState::render()
{
    window.setView(menuView);
    window.draw(gameTitle);

    for (const auto &button : buttons)
    {
        button.render(window);
    }
}

// Privates
void MainMenuState::updateMenuItemPositions()
{
    sf::Vector2f viewCenter = menuView.getCenter();
    sf::Vector2f viewSize = menuView.getSize();

    gameTitle.setPosition(viewCenter.x - viewSize.x / 2.f + 25.f,
                          viewCenter.y - viewSize.y / 2.f + 25.f);

    float offsetX = viewCenter.x - (Constants::BUTTON_WIDTH / 2.0f);
    buttons[0].setPosition(sf::Vector2f(offsetX, viewCenter.y - (buttonSpacing * 1.5f)));
    buttons[1].setPosition(sf::Vector2f(offsetX, viewCenter.y - (buttonSpacing / 2.0f)));
    buttons[2].setPosition(sf::Vector2f(offsetX, viewCenter.y + (buttonSpacing / 2.0f)));
    buttons[3].setPosition(sf::Vector2f(offsetX, viewCenter.y + (buttonSpacing * 1.5f)));
}
