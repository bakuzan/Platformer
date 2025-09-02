#include "utils/GameUtils.h"
#include "utils/InputUtils.h"

#include "UIManager.h"

UIManager::UIManager(sf::RenderWindow *gameWindow, const GameData &data)
    : window(gameWindow),
      gameData(data)
{
}

UIManager::~UIManager()
{
    // Destructor
}

// Publics

void UIManager::handleEvent(sf::Event event)
{
    (void)event;

    sf::View prevView = window->getView();
    window->setView(window->getDefaultView()); // Switch to UI view

    window->setView(prevView); // Restore previous view
}

void UIManager::update()
{
}

void UIManager::render()
{
    sf::View prevView = window->getView();
    window->setView(window->getDefaultView()); // Switch to UI view

    window->setView(prevView); // Restore previous view
}
