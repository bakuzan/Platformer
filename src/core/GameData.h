#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/Graphics.hpp>

#include <optional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "TextureManager.h"
#include "AudioManager.h"
#include "components/TextureRectManager.h"
#include "data/RoomData.h"
#include "entities/Item.h"
#include "entities/Player.h"

class GameData
{
private:
    std::shared_ptr<Player> player;
    RoomData roomData;

    std::vector<std::unique_ptr<Item>> items;

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

    std::vector<std::unique_ptr<Item>> &getItems();

    const RoomData &getRoomData() const;
    void setRoomData(RoomData data);

    void resetLevel();
    void reset();
};

#endif // GAMEDATA_H
