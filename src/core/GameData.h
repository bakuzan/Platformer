#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

#include "TextureManager.h"
#include "AudioManager.h"
#include "components/TextureRectManager.h"
#include "data/RoomData.h"
#include "entities/Player.h"

class GameData
{
private:
    std::shared_ptr<Player> player;
    RoomData roomData;

public:
    sf::Font gameFont;

    TextureManager textureManager;
    AudioManager audioManager;

    TextureRectManager rectManager;

public:
    GameData();
    ~GameData();

    std::shared_ptr<Player> getPlayer() const;
    void setPlayer(std::shared_ptr<Player> p);

    const RoomData &getRoomData() const;
    void setRoomData(const RoomData &data);

    void resetLevel();
    void reset();
};

#endif // GAMEDATA_H
