#include "utils/GameUtils.h"
#include "utils/InputUtils.h"

#include "UIManager.h"

UIManager::UIManager(sf::RenderWindow *gameWindow, const GameData &data)
    : window(gameWindow),
      gameData(data),
      isTooltipVisible(false)
{
    tooltipText.setFont(gameData.gameFont);
    tooltipText.setCharacterSize(16);
    tooltipText.setFillColor(sf::Color::White);
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

    if (isTooltipVisible)
    {
        window->draw(tooltipText);
    }

    window->setView(prevView); // Restore previous view
}

// Tooltip API
void UIManager::showTooltip(const std::string &text, sf::Vector2f pos)
{
    tooltipText.setString(text);
    tooltipText.setPosition(pos);
    isTooltipVisible = true;
}

void UIManager::clearTooltip()
{
    isTooltipVisible = false;
}
