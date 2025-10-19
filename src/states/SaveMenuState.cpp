#include <memory>
#include <format>

#include "constants/Constants.h"
#include "utils/InputUtils.h"
#include "SaveMenuState.h"
#include "SaveMenuState.h"

SaveMenuState::SaveMenuState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win)
{
    sf::Vector2f viewSize = saveView.getSize();
    sf::Vector2f center(saveView.getCenter());
    window.setView(saveView);

    // Configure background
    background.setSize(sf::Vector2f(viewSize.x - 20.0f, viewSize.y - 20.0f));
    background.setFillColor(sf::Color(0, 0, 0, 150));

    // Configure text
    saveText.setFont(gameData.gameFont);
    saveText.setString("Saves");
    saveText.setCharacterSize(36);
    saveText.setFillColor(sf::Color::White);

    // To ensure positioning is updated relative to window resizing
    updateMenuItemPositions();
}

SaveMenuState::~SaveMenuState()
{
    // Destructor
}

// Publics

void SaveMenuState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed &&
        InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Escape}))
    {
        stateManager.popState(); // Resume the game by returning to GameState
    }
}

void SaveMenuState::handleWindowResize(sf::Vector2u newSize)
{
    (void)newSize;
    updateMenuItemPositions();
}

void SaveMenuState::update(sf::Time deltaTime)
{
    (void)deltaTime;

    gameData.audioManager.cleanupSounds();
}

void SaveMenuState::render()
{
    window.setView(saveView);
    window.draw(background);
    window.draw(saveText);
}

// Privates
void SaveMenuState::updateMenuItemPositions()
{
    sf::Vector2f viewCenter = saveView.getCenter();
    sf::Vector2f backgroundSize = background.getSize();

    background.setPosition(
        viewCenter.x - backgroundSize.x / 2.f,
        viewCenter.y - backgroundSize.y / 2.f);

    saveText.setPosition(
        background.getPosition().x + 10.f,
        background.getPosition().y + 20.f);
}