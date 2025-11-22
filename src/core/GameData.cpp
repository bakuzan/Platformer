#include <utility>

#include "GameData.h"
#include "constants/Constants.h"
#include "constants/AudioId.h"
#include "components/SettingsManager.h"

GameData::GameData()
{
    // Load textures

    // Load audio

    // Load font
    if (!gameFont.loadFromFile("resources/fonts/PressStart2P-Regular.ttf"))
    {
        throw std::runtime_error("Failed to load font");
    }

    reset();
}

GameData::~GameData()
{
    // Destructor
}

// Publics

std::shared_ptr<Player> GameData::getPlayer() const
{
    return player;
}

void GameData::setPlayer(std::shared_ptr<Player> p)
{
    player = std::move(p);
}

std::vector<std::unique_ptr<Item>> &GameData::getItems()
{
    return items;
}

std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>> &GameData::getDestroyedTiles()
{
    return destroyedTiles;
}

std::unordered_set<TileKey, TileKeyHash> &GameData::getDestroyedRoomTiles(const std::string &filename)
{
    return destroyedTiles[filename];
}

const RoomData &GameData::getRoomData() const
{
    return roomData;
}

void GameData::setRoomData(RoomData data)
{
    roomData = std::move(data);
}

void GameData::markDestroyedTile(const std::string &fileName, int tileX, int tileY)
{
    destroyedTiles[fileName].insert({tileX, tileY});
}

// Reset methods for cleaning up
void GameData::resetLevel()
{
    items.clear();
}

void GameData::reset()
{
    resetLevel();
    destroyedTiles.clear();
}