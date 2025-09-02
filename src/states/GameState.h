#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

#include "core/GameData.h"
#include "core/State.h"
#include "core/StateManager.h"

#include "components/UIManager.h"

class GameState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;
    sf::View view;

    UIManager uiManager;

    int levelIndex;

private:
    void loadMap(const std::string filename);
    void onPlayerDeath();

public:
    GameState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~GameState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime) override;
    void render() override;
};

#endif // GAMESTATE_H
