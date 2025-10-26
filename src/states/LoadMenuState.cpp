#include "states/GameState.h"
#include "LoadMenuState.h"

LoadMenuState::LoadMenuState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : SaveLoadMenuState(data, manager, window, false)
{
    confirmText.setString("Are you sure you want to load?");
}

LoadMenuState::~LoadMenuState()
{
    // Destructor
}

// Privates

void LoadMenuState::onSlotAction(int slot)
{
    gameData.reset();

    auto saveData = saveManager.loadSlot(slot);
    // TODO pass saveData...
    stateManager.replaceStates(
        std::make_unique<GameState>(gameData, stateManager, window));
}
