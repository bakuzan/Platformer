#include <SFML/Graphics.hpp>

#include "entities/enemy/ChargerEnemy.h"
#include "entities/enemy/FlierEnemy.h"
#include "entities/enemy/SlugEnemy.h"
#include "entities/enemy/SwimmerEnemy.h"

#include "constants/EnemyType.h"
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

void RoomData::processRoomEntities(
    std::shared_ptr<Player> player,
    std::vector<std::unique_ptr<Item>> &items,
    std::vector<std::unique_ptr<Enemy>> &enemies) const
{
    for (const auto &e : entities)
    {
        if (e.type == "Enemy")
        {
            processEnemy(enemies, e);
        }
        else if (e.type == "PlayerAbility")
        {
            processPlayerAbility(player, items, e);
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

void RoomData::processEnemy(
    std::vector<std::unique_ptr<Enemy>> &enemies,
    RoomEntity entity) const
{
    EnemyType enemyInt = static_cast<EnemyType>(std::stoi(entity.properties.at("type")));

    switch (enemyInt)
    {
    case EnemyType::SLUG:
    {
        int leftX = std::stoi(entity.properties.at("patrolLeftX"));
        int rightX = std::stoi(entity.properties.at("patrolRightX"));
        sf::Vector2f pos(entity.x * tileSize,
                         entity.y * tileSize);

        enemies.push_back(std::make_unique<SlugEnemy>(pos,
                                                      leftX * tileSize,
                                                      rightX * tileSize));
        break;
    }
    case EnemyType::CHARGER:
    {
        int leftX = std::stoi(entity.properties.at("patrolLeftX"));
        int rightX = std::stoi(entity.properties.at("patrolRightX"));
        sf::Vector2f pos(entity.x * tileSize,
                         entity.y * tileSize);

        enemies.push_back(std::make_unique<ChargerEnemy>(pos,
                                                         leftX * tileSize,
                                                         rightX * tileSize));
        break;
    }
    case EnemyType::FLIER:
    {
        int leftX = std::stoi(entity.properties.at("patrolLeftX"));
        int rightX = std::stoi(entity.properties.at("patrolRightX"));
        sf::Vector2f pos(entity.x * tileSize,
                         entity.y * tileSize);

        enemies.push_back(std::make_unique<FlierEnemy>(pos,
                                                       leftX * tileSize,
                                                       rightX * tileSize));
        break;
    }
    case EnemyType::SWIMMER:
    {
        int leftX = std::stoi(entity.properties.at("patrolLeftX"));
        int rightX = std::stoi(entity.properties.at("patrolRightX"));
        int topY = std::stoi(entity.properties.at("patrolTopY"));
        int bottomY = std::stoi(entity.properties.at("patrolBottomY"));
        sf::Vector2f pos(entity.x * tileSize,
                         entity.y * tileSize);

        enemies.push_back(std::make_unique<SwimmerEnemy>(pos,
                                                         leftX * tileSize,
                                                         rightX * tileSize,
                                                         topY * tileSize,
                                                         bottomY * tileSize));
        break;
    }
    default:
        break;
    }
}

void RoomData::processPlayerAbility(
    std::shared_ptr<Player> player,
    std::vector<std::unique_ptr<Item>> &items,
    RoomEntity entity) const
{
    int abilityInt = std::stoi(entity.properties.at("type"));
    PlayerAbility ability = static_cast<PlayerAbility>(abilityInt);

    if (!player->hasAbility(ability))
    {
        items.push_back(std::make_unique<PowerUp>(entity.x * tileSize,
                                                  entity.y * tileSize,
                                                  ability));
    }
}