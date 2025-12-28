#include <iostream>

#include "utils/GameUtils.h"
#include "utils/InputUtils.h"

#include "UIManager.h"

UIManager::UIManager(sf::RenderWindow *gameWindow, const GameData &data)
    : window(gameWindow),
      gameData(data),
      isTooltipVisible(false)
{
    sf::Vector2u windowSize = window->getSize();
    handleResize(windowSize.x, windowSize.y);
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
    window->setView(uiView); // Switch to UI view

    // Make changes

    window->setView(prevView); // Restore previous view
}

void UIManager::handleResize(unsigned int windowWidth, unsigned int windowHeight)
{
    uiView.reset(sf::FloatRect(0.f, 0.f,
                               static_cast<float>(windowWidth),
                               static_cast<float>(windowHeight)));
}

void UIManager::update(Player &player)
{
    updateHealthBar(player.getHealth(), player.getMaxHealth());
}

void UIManager::render()
{
    sf::View prevView = window->getView();
    window->setView(uiView); // Switch to UI view

    window->draw(healthBarBg);
    window->draw(healthBarFg);

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

    sf::Vector2i pixelPos = window->mapCoordsToPixel(pos, window->getView());
    sf::FloatRect bounds = tooltipText.getLocalBounds();

    tooltipText.setOrigin(bounds.width / 2.f, bounds.height);
    tooltipText.setPosition(static_cast<float>(pixelPos.x),
                            static_cast<float>(pixelPos.y) - 20.f);

    isTooltipVisible = true;
}

void UIManager::clearTooltip()
{
    isTooltipVisible = false;
}

// Privates

void UIManager::updateHealthBar(int health, int maxHealth)
{
    float ratio = static_cast<float>(health) / maxHealth;

    // --- Background bar ---
    healthBarBg.setSize({260.f, 36.f});
    healthBarBg.setFillColor(sf::Color(118, 118, 117));
    healthBarBg.setPosition(10.f, 10.f);

    // --- Foreground bar ---
    healthBarFg.setSize({(260.f - 6.f) * ratio, 30.f}); // inset + scaled
    healthBarFg.setPosition(13.f, 13.f);

    // Color selection
    if (ratio > 0.7f)
    {
        healthBarFg.setFillColor(sf::Color::Green);
    }
    else if (ratio > 0.4f)
    {
        healthBarFg.setFillColor(sf::Color::Yellow);
    }
    else if (ratio > 0.2f)
    {
        healthBarFg.setFillColor(sf::Color(255, 165, 0)); // Orange
    }
    else
    {
        healthBarFg.setFillColor(sf::Color::Red);
    }
}
