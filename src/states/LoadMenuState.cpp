#include "data/SaveData.h"
#include "states/GameState.h"
#include "LoadMenuState.h"

LoadMenuState::LoadMenuState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : SaveLoadMenuState(data, manager, window, false)
{
    confirmText.setString("Are you sure you want to load?");
    background.setFillColor(sf::Color(0, 0, 0, 220));
}

LoadMenuState::~LoadMenuState()
{
    // Destructor
}

// Privates

void LoadMenuState::onSlotAction(int slot)
{
    gameData.reset();

    SaveData saveData = saveManager.loadSlot(slot);
    stateManager.replaceStates(
        std::make_unique<GameState>(gameData, stateManager, window, saveData));
}
