#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>

#include "core/GameData.h"

class UIManager
{
private:
    sf::RenderWindow *window;
    const GameData &gameData;

    bool isTooltipVisible;
    sf::Text tooltipText;

public:
    UIManager(sf::RenderWindow *gameWindow, const GameData &data);
    ~UIManager();

    void handleEvent(sf::Event event);
    void update();
    void render();

    void showTooltip(const std::string &text, sf::Vector2f pos);
    void clearTooltip();
};

#endif // UIMANAGER_H
