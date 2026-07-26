#include <iostream>
#include <format>

#include "constants/Constants.h"
#include "utils/GameUtils.h"
#include "utils/InputUtils.h"

#include "UIManager.h"

UIManager::UIManager(sf::RenderWindow *gameWindow, const GameData &data)
    : window(gameWindow),
      gameData(data),
      miniMap(Constants::MINI_MAP_SIZE, Constants::MINI_MAP_SIZE),
      isTooltipVisible(false),
      playerHealthBar({Constants::HEALTH_BAR_BACKGROUND_WIDTH, Constants::HEALTH_BAR_BACKGROUND_HEIGHT},
                      3.0f)
{
    sf::Vector2u windowSize = window->getSize();
    handleResize(windowSize.x, windowSize.y);

    tooltipText.setFont(gameData.gameFont);
    tooltipText.setCharacterSize(16);
    tooltipText.setFillColor(sf::Color::White);
    tooltipText.setOutlineThickness(2);
    tooltipText.setOutlineColor(sf::Color::Black);

    ammoText.setFont(gameData.gameFont);
    ammoText.setCharacterSize(16);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setOutlineThickness(1.5f);
    ammoText.setOutlineColor(sf::Color::Black);

    float ammoY = 10.f + Constants::HEALTH_BAR_BACKGROUND_HEIGHT + 8.f;
    ammoText.setPosition(13.f, ammoY);
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

void UIManager::update(TileMap &tileMap, Player &player)
{
    updateHealthBar(player.getHealth(), player.getMaxHealth());
    updateAmmoDisplay(player);
    miniMap.update(uiView, gameData, tileMap);
}

void UIManager::render(TileMap &tileMap)
{
    sf::View prevView = window->getView();
    window->setView(uiView); // Switch to UI view

    playerHealthBar.render(*window);
    window->draw(ammoText);

    if (isTooltipVisible)
    {
        window->draw(tooltipText);
    }

    miniMap.render(*window, gameData, tileMap);

    window->setView(prevView); // Restore previous view
}

// Tooltip API
void UIManager::showTooltip(const std::string &text, sf::Vector2f pos)
{
    tooltipText.setString(text);

    sf::Vector2f uiCoords = window->mapPixelToCoords(
        window->mapCoordsToPixel(pos, window->getView()),
        uiView);
    sf::FloatRect bounds = tooltipText.getLocalBounds();

    tooltipText.setOrigin(bounds.width / 2.f, bounds.height);
    tooltipText.setPosition(uiCoords.x,
                            uiCoords.y - 20.f);

    isTooltipVisible = true;
}

void UIManager::clearTooltip()
{
    isTooltipVisible = false;
}

// Privates

void UIManager::updateHealthBar(int health, int maxHealth)
{
    playerHealthBar.update(health, maxHealth, {10.f, 10.f});
}

void UIManager::updateAmmoDisplay(const Player &player)
{
    ProjectileType currentWeapon = player.getCurrentSecondaryType();

    if (currentWeapon == ProjectileType::NONE)
    {
        ammoText.setString("");
        return;
    }

    int ammoCount = player.getAmmo(currentWeapon);
    std::string weaponName = EnumUtils::enumToString(currentWeapon);
    sf::Color ammoTextColour = ammoCount <= 0
                                   ? sf::Color(255, 100, 100)
                                   : Constants::ammoItemColour;

    ammoText.setString(std::format("{}: {}", weaponName, ammoCount));
    ammoText.setFillColor(ammoTextColour);
}