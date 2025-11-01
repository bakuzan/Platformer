#include "RoomData.h"

RoomData::RoomData(const std::string filename, float roomTileSize)
    : fileName(filename), tileSize(roomTileSize)
{
}

RoomData::~RoomData()
{
    // Destructor
}

// Publics

sf::Vector2f RoomData::getRoomDimensions() const
{
    if (tileGrid.size() == 0)
    {
        return sf::Vector2f(0.0f, 0.0f);
    }

    return sf::Vector2f(static_cast<int>(tileGrid[0].size()) * tileSize,
                        static_cast<int>(tileGrid.size()) * tileSize);
}

sf::Vector2f RoomData::getPlayerSpawn(const std::string &spawnKey) const
{
    const std::vector<const std::vector<RoomEntity> *> sources =
        {
            &entities,
            &entrances,
            &savePoints};

    for (const auto *list : sources)
    {
        for (const auto &e : *list)
        {
            auto it = e.properties.find("spawn");

            if (it != e.properties.end() &&
                it->second == spawnKey)
            {
                return resolveSpawn(e);
            }
        }
    }

    throw std::runtime_error("No spawn found for key: " + spawnKey);
}

void RoomData::processRoomItems(std::vector<std::unique_ptr<Item>> &items,
                                std::shared_ptr<Player> player) const
{
    for (const auto &e : entities)
    {
        if (e.type == "PlayerAbility")
        {
            int abilityInt = std::stoi(e.properties.at("type"));
            PlayerAbility ability = static_cast<PlayerAbility>(abilityInt);

            if (!player->hasAbility(ability))
            {
                items.push_back(std::make_unique<PowerUp>(e.x * tileSize,
                                                          e.y * tileSize,
                                                          ability));
            }
        }
    }
}

// Privates

sf::Vector2f RoomData::resolveSpawn(const RoomEntity &e) const
{
    int sx = e.x;
    int sy = e.y;

    if (auto sxIt = e.properties.find("spawnX"); sxIt != e.properties.end())
    {
        sx = std::stoi(sxIt->second);
    }

    if (auto syIt = e.properties.find("spawnY"); syIt != e.properties.end())
    {
        sy = std::stoi(syIt->second);
    }

    return {sx * tileSize,
            sy * tileSize};
}