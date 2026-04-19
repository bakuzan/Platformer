#include <memory>
#include <format>

#include "constants/Constants.h"
#include "utils/InputUtils.h"
#include "LoadMenuState.h"
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
    addButton("Resume", "Resume",
              [this]()
              { stateManager.popState(); });
    addButton("Load", "Load Game",
              [this]()
              { stateManager.pushState(std::make_unique<LoadMenuState>(gameData, stateManager, window)); });
    addButton("Quit", "Quit Game",
              [this]()
              { gameData.reset();
                stateManager.replaceStates(std::make_unique<MainMenuState>(gameData, stateManager, window)); });
    addButton("Exit", "Exit",
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

void GameMenuState::addButton(std::string id,
                              std::string label,
                              std::function<void()> callback)
{
    buttons.emplace_back(id, gameData.gameFont, label, sf::Vector2f(0.f, 0.f), callback);
}

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

    if (buttons.empty())
    {
        return;
    }

    float offsetX = viewCenter.x - (Constants::BUTTON_WIDTH / 2.0f);
    float totalHeight = (buttons.size() - 1) * buttonSpacing;
    float startY = viewCenter.y - (totalHeight / 2.0f);

    for (size_t i = 0; i < buttons.size(); ++i)
    {
        float yPos = startY + (i * buttonSpacing);
        buttons[i].setPosition(sf::Vector2f(offsetX, yPos));
    }
}