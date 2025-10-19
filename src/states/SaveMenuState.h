#ifndef SAVEMENUSTATE_H
#define SAVEMENUSTATE_H

#include <SFML/Graphics.hpp>

#include "ui/Button.h"
#include "core/GameData.h"
#include "core/State.h"
#include "core/StateManager.h"

class SaveMenuState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;
    sf::View saveView;

    sf::RectangleShape background;
    sf::Text saveText;

private:
    void updateMenuItemPositions();

public:
    SaveMenuState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~SaveMenuState();

    void handleEvent(const sf::Event &event) override;
    void handleWindowResize(sf::Vector2u newSize) override;
    void update(sf::Time deltaTime) override;
    void render() override;
};

#endif // SAVEMENUSTATE_H
