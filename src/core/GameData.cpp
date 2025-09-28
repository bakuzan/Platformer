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

const RoomData &GameData::getRoomData() const
{
    return roomData;
}

void GameData::setRoomData(const RoomData &data)
{
    roomData = std::move(data);
}

// Reset methods for cleaning up
void GameData::resetLevel()
{
}

void GameData::reset()
{
    resetLevel();
}