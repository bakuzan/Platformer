#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

#include "TextureManager.h"
#include "AudioManager.h"
#include "components/TextureRectManager.h"

class GameData
{
private:
    std::vector<std::string> levelMapPaths;

public:
    sf::Font gameFont;

    TextureManager textureManager;
    AudioManager audioManager;

    TextureRectManager rectManager;

public:
    GameData();
    ~GameData();

    std::string getLevelMap(int index);
    bool hasLevelMap(int index);

    void resetLevel();
    void reset();
};

#endif // GAMEDATA_H
