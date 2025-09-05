#include <memory>
#include <format>

#include "constants/Constants.h"
#include "utils/InputUtils.h"
#include "MainMenuState.h"
#include "GameMenuState.h"

GameMenuState::GameMenuState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win)
{
    buttonSpacing = Constants::BUTTON_HEIGHT + 10.f;
    sf::Vector2f center(pauseView.getCenter());
    window.setView(pauseView);

    // Configure background
    background.setSize(sf::Vector2f(300.f, 300.f));
    background.setFillColor(sf::Color(0, 0, 0, 150));

    // Configure text
    pauseText.setFont(gameData.gameFont);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(36);
    pauseText.setFillColor(sf::Color::White);

    // Add buttons
    buttons.emplace_back("Resume", gameData.gameFont, "Resume", sf::Vector2f(center.x - 100.f, center.y - buttonSpacing),
                         [this]()
                         { stateManager.popState(); });
    buttons.emplace_back("Quit", gameData.gameFont, "Quit Game", sf::Vector2f(center.x - 100.f, center.y),
                         [this]()
                         {
                             gameData.reset();
                             stateManager.replaceStates(std::make_unique<MainMenuState>(gameData, stateManager, window));
                         });
    buttons.emplace_back("Exit", gameData.gameFont, "Exit", sf::Vector2f(center.x - 100.f, center.y + buttonSpacing),
                         [this]()
                         { window.close(); });

    // To ensure positioning is updated relative to window resizing
    updateMenuItemPositions();
}

GameMenuState::~GameMenuState()
{
    // Destructor
}

// Publics

void GameMenuState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed &&
        InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Escape, sf::Keyboard::P}))
    {
        stateManager.popState(); // Resume the game by returning to GameState
    }

    InputUtils::handleButtonEvent(event, buttons, window, selectedButtonIndex);
}

void GameMenuState::handleWindowResize(sf::Vector2u newSize)
{
    (void)newSize;
    updateMenuItemPositions();
}

void GameMenuState::update(sf::Time deltaTime)
{
    (void)deltaTime;

    gameData.audioManager.cleanupSounds();
}

void GameMenuState::render()
{
    window.setView(pauseView);
    window.draw(background);
    window.draw(pauseText);

    for (const auto &button : buttons)
    {
        button.render(window);
    }
}

// Privates
void GameMenuState::updateMenuItemPositions()
{
    sf::Vector2f viewCenter = pauseView.getCenter();
    sf::Vector2f backgroundSize = background.getSize();

    background.setPosition(
        viewCenter.x - backgroundSize.x / 2.f,
        viewCenter.y - backgroundSize.y / 2.f);

    pauseText.setPosition(
        background.getPosition().x + (backgroundSize.x - pauseText.getGlobalBounds().width) / 2.f,
        background.getPosition().y + 20.f);

    float offsetX = Constants::BUTTON_WIDTH / 2.0f;
    buttons[0].setPosition(sf::Vector2f(viewCenter.x - offsetX, viewCenter.y - buttonSpacing));
    buttons[1].setPosition(sf::Vector2f(viewCenter.x - offsetX, viewCenter.y));
    buttons[2].setPosition(sf::Vector2f(viewCenter.x - offsetX, viewCenter.y + buttonSpacing));
}