#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>

#include "core/GameData.h"

class UIManager
{
private:
    sf::RenderWindow *window;
    const GameData &gameData;

public:
    UIManager(sf::RenderWindow *gameWindow, const GameData &data);
    ~UIManager();

    void handleEvent(sf::Event event);
    void update();
    void render();
};

#endif // UIMANAGER_H
