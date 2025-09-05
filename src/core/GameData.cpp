#include "GameData.h"
#include "constants/Constants.h"
#include "constants/AudioId.h"
#include "components/SettingsManager.h"

GameData::GameData()
{
    // Load textures

    // Load audio

    // Load maps
    levelMapPaths.push_back("resources/maps/room_01.txt");

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

std::string GameData::getLevelMap(int index)
{
    if (hasLevelMap(index))
    {
        return levelMapPaths[index];
    }

    return levelMapPaths[0]; // Fallback
}

bool GameData::hasLevelMap(int index)
{
    return index >= 0 &&
           index < static_cast<int>(levelMapPaths.size());
}

std::shared_ptr<Player> GameData::getPlayer() const
{
    return player;
}

void GameData::setPlayer(std::shared_ptr<Player> p)
{
    player = std::move(p);
}

const RoomData &GameData::getRoomData() const
{
    return roomData;
}

void GameData::setRoomData(const RoomData &data)
{
    roomData = data;
}

// Reset methods for cleaning up
void GameData::resetLevel()
{
}

void GameData::reset()
{
    resetLevel();
}