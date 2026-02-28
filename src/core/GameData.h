#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/Graphics.hpp>

#include <optional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "TextureManager.h"
#include "components/TextureRectManager.h"
#include "data/RoomData.h"
#include "data/TileKey.h"
#include "entities/Enemy.h"
#include "entities/Item.h"
#include "entities/Player.h"

class GameData
{
private:
    std::shared_ptr<Player> player;
    RoomData roomData;

    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>> destroyedTiles;

public:
    sf::Font gameFont;

    TextureManager textureManager;

    TextureRectManager rectManager;

public:
    GameData();
    ~GameData();

    std::shared_ptr<Player> getPlayer() const;
    void setPlayer(std::shared_ptr<Player> p);

    std::vector<std::unique_ptr<Item>> &getItems();
    std::vector<std::unique_ptr<Enemy>> &getEnemies();
    std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>> &getDestroyedTiles();
    std::unordered_set<TileKey, TileKeyHash> &getDestroyedRoomTiles(const std::string &filename);

    const RoomData &getRoomData() const;
    void setRoomData(RoomData data);
    void markDestroyedTile(const std::string &fileName, int tileX, int tileY);

    void resetLevel();
    void reset();
};

#endif // GAMEDATA_H
