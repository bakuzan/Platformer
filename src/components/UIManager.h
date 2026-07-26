#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>

#include "core/GameData.h"
#include "components/MiniMap.h"
#include "components/TileMap.h"
#include "entities/Player.h"
#include "ui/HealthBar.h"

class UIManager
{
private:
    sf::RenderWindow *window;
    sf::View uiView;
    const GameData &gameData;
    MiniMap miniMap;

    bool isTooltipVisible;
    sf::Text tooltipText;

    HealthBar playerHealthBar;

    sf::Text ammoText;
    sf::RectangleShape cooldownBarBg;
    sf::RectangleShape cooldownBarFill;
    const float COOLDOWN_BAR_HEIGHT = 4.f;

private:
    void updateHealthBar(int health, int maxHealth);
    void updateAmmoDisplay(const Player &player);

public:
    UIManager(sf::RenderWindow *gameWindow, const GameData &data);
    ~UIManager();

    void handleEvent(sf::Event event);
    void handleResize(unsigned int windowWidth, unsigned int windowHeight);
    void update(TileMap &tileMap, Player &player);
    void render(TileMap &tileMap);

    void showTooltip(const std::string &text, sf::Vector2f pos);
    void clearTooltip();
};

#endif // UIMANAGER_H
