#include "GameData.h"
#include "constants/Constants.h"
#include "constants/AudioId.h"
#include "components/SettingsManager.h"

GameData::GameData()
{
    // Load textures

    // Load audio

    // Load maps

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

void GameData::resetLevel()
{
}

void GameData::reset()
{
    resetLevel();
}