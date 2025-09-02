#include "SettingsState.h"
#include "MainMenuState.h"

#include "components/SettingsManager.h"
#include "constants/Constants.h"
#include "utils/InputUtils.h"

SettingsState::SettingsState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win),
      selectedButtonIndex(0),
      shouldReturnToMenuState(false)
{
    buttonSpacing = Constants::BUTTON_HEIGHT + 10.f;
    window.setView(settingsView);
    sf::Vector2f viewSize = settingsView.getSize();

    // Setup title
    gameTitle.setFont(gameData.gameFont);
    gameTitle.setString("Settings");
    gameTitle.setCharacterSize(48);
    gameTitle.setFillColor(sf::Color::White);

    // Add buttons
    buttons.emplace_back("Save", gameData.gameFont, "Save",
                         sf::Vector2f(viewSize.x - Constants::BUTTON_WIDTH - 10.0f, viewSize.y - buttonSpacing),
                         [this]()
                         { SettingsManager::getInstance().save();
                            shouldReturnToMenuState=true; });
    buttons.emplace_back("Back", gameData.gameFont, "Back",
                         sf::Vector2f(10.0f, viewSize.y - buttonSpacing),
                         [this]()
                         { SettingsManager::getInstance().reset();
                            shouldReturnToMenuState=true; });

    // To ensure positioning is updated relative to window resizing
    updateMenuItemPositions();
}

SettingsState::~SettingsState()
{
    // Destructor
}

// Publics

void SettingsState::handleEvent(const sf::Event &event)
{
    InputUtils::handleButtonEvent(event, buttons, window, selectedButtonIndex);

    if (event.type == sf::Event::Resized)
    {
        updateMenuItemPositions();
    }
}

void SettingsState::update(sf::Time deltaTime)
{
    (void)deltaTime;

    if (shouldReturnToMenuState)
    {
        stateManager.changeState(std::make_unique<MainMenuState>(gameData, stateManager, window));
    }
}

void SettingsState::render()
{
    window.setView(settingsView);
    window.draw(gameTitle);

    for (const auto &button : buttons)
    {
        button.render(window);
    }
}

// Privates

void SettingsState::updateMenuItemPositions()
{
    sf::Vector2f viewCenter = settingsView.getCenter();
    sf::Vector2f viewSize = settingsView.getSize();

    gameTitle.setPosition(viewCenter.x - viewSize.x / 2.f + 25.f,
                          viewCenter.y - viewSize.y / 2.f + 25.f);

    buttons[0].setPosition(sf::Vector2f(viewSize.x - Constants::BUTTON_WIDTH - 10.0f, viewSize.y - buttonSpacing));
    buttons[1].setPosition(sf::Vector2f(10.0f, viewSize.y - buttonSpacing));
}
