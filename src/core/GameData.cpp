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

std::vector<std::unique_ptr<Enemy>> &GameData::getEnemies()
{
    return enemies;
}

std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>> &GameData::getDestroyedTiles()
{
    return destroyedTiles;
}

std::unordered_set<TileKey, TileKeyHash> &GameData::getDestroyedRoomTiles(const std::string &mapRoomId)
{
    return destroyedTiles[mapRoomId];
}

const std::unordered_map<std::string, std::vector<std::vector<bool>>> &GameData::getRevealedTiles() const
{
    return revealedTiles;
}

const std::vector<std::vector<bool>> &GameData::getRevealedRoomTiles(const std::string &mapRoomId) const
{
    return revealedTiles.at(mapRoomId);
}

const RoomData &GameData::getRoomData() const
{
    return roomData;
}

void GameData::setRoomData(RoomData data)
{
    roomData = std::move(data);
    sf::Vector2i gridDims = roomData.getRoomGridDimensions();
    initRevealGrid(roomData.roomId, gridDims.x, gridDims.y);
}

const WorldData &GameData::getWorldData() const
{
    return worldData;
}

void GameData::setWorldData(WorldData data)
{
    worldData = std::move(data);
}

void GameData::markDestroyedTile(const std::string &mapRoomId, int tileX, int tileY)
{
    destroyedTiles[mapRoomId].insert({tileX, tileY});
}

void GameData::setRevealedTiles(const std::string &mapRoomId, std::vector<std::vector<bool>> revealed)
{
    revealedTiles[mapRoomId] = revealed;
}

void GameData::revealTile(const std::string &mapRoomId, int tileX, int tileY)
{
    auto &grid = revealedTiles[mapRoomId];
    if (tileY >= 0 && tileY < grid.size() &&
        tileX >= 0 && tileX < grid[0].size())
    {
        grid[tileY][tileX] = true;
    }
}

// Reset methods for cleaning up
void GameData::resetLevel()
{
    items.clear();
    enemies.clear();
}

void GameData::reset()
{
    resetLevel();
    destroyedTiles.clear();
    revealedTiles.clear();
}

// Privates
void GameData::initRevealGrid(const std::string &mapRoomId, int width, int height)
{
    auto &grid = revealedTiles[mapRoomId];

    if (grid.empty())
    {
        grid.assign(height, std::vector<bool>(width, false));
    }
}
