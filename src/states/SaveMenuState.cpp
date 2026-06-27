#include "utils/DataUtils.h"
#include "SaveMenuState.h"

SaveMenuState::SaveMenuState(GameData &data, StateManager &manager, sf::RenderWindow &window,
                             const std::string spawnName)
    : SaveLoadMenuState(data, manager, window, true),
      currentSpawnName(spawnName)
{
    confirmText.setString("Are you sure you want to save?");
}

SaveMenuState::~SaveMenuState()
{
    // Destructor
}

// Privates

void SaveMenuState::onSlotAction(int slot)
{
    auto worldData = gameData.getWorldData();
    auto roomData = gameData.getRoomData();
    auto player = gameData.getPlayer();

    auto levelId = worldData.roomToLevel[roomData.roomId];
    auto levelName = worldData.levels[levelId].name;
    auto roomId = roomData.roomId;

    SaveData data;
    data.timestamp = DataUtils::currentTimestamp();
    data.room = roomId;
    data.locationName = "Area " + levelName;
    data.spawn = currentSpawnName;
    data.playerAbilities = player->getCurrentAbilities();
    data.destroyedTiles = gameData.getDestroyedTiles();
    data.revealedTiles = gameData.getRevealedTiles();

    saveManager.saveSlot(slot, data);
    player->healDamage(static_cast<float>(player->getMaxHealth()));

    stateManager.popState();
}
