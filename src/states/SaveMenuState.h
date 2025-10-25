#ifndef SAVEMENUSTATE_H
#define SAVEMENUSTATE_H

#include <SFML/Graphics.hpp>

#include <array>

#include "ui/Button.h"
#include "components/SaveManager.h"
#include "core/GameData.h"
#include "core/State.h"
#include "core/StateManager.h"
#include "data/SaveSlot.h"

class SaveMenuState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;
    sf::View saveView;

    SaveManager saveManager;
    const std::string currentSpawnName;

    sf::RectangleShape background;
    sf::Text saveText;

    // Save slots
    std::array<SaveSlot, 3> slots;
    int highlightedSlot = 0;

    // Confirmation overlay
    bool showingConfirm = false;
    sf::RectangleShape confirmBox;
    sf::Text confirmText;
    std::array<sf::Text, 2> confirmButtons; // [0] = Yes, [1] = No
    int highlightedConfirm = 1;             // default = No

private:
    void updateMenuItemPositions();

    void refreshSlots();
    void moveHighlight(int dir);
    void selectSlot();
    void showConfirmation();
    void handleConfirmInput(int dir);
    void confirmSelection();

    int hitTestConfirm(sf::Vector2f mousePos);
    int hitTestSlots(sf::Vector2f mousePos);

    void save(int slot);

public:
    SaveMenuState(GameData &data, StateManager &manager, sf::RenderWindow &window,
                  const std::string spawnName);
    ~SaveMenuState();

    void handleEvent(const sf::Event &event) override;
    void handleWindowResize(sf::Vector2u newSize) override;
    void update(sf::Time deltaTime) override;
    void render() override;
};

#endif // SAVEMENUSTATE_H
