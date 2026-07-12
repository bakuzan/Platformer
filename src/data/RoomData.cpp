#include <SFML/Graphics.hpp>
#include <iostream>

#include "constants/Constants.h"
#include "constants/EnemyType.h"

#include "entities/enemy/ChargerEnemy.h"
#include "entities/enemy/FlierEnemy.h"
#include "entities/enemy/SlugEnemy.h"
#include "entities/enemy/SwimmerEnemy.h"
#include "entities/enemy/CrawlerEnemy.h"
#include "utils/EnumUtils.h"

#include "RoomData.h"

RoomData::RoomData(const std::string roomId, const std::string filename, float roomTileSize)
    : roomId(roomId), fileName(filename), tileSize(roomTileSize)
{
}

RoomData::~RoomData()
{
    // Destructor
}

// Publics

sf::Vector2i RoomData::getRoomGridDimensions() const
{
    if (tileGrid.size() == 0)
    {
        return sf::Vector2i(0, 0);
    }

    return sf::Vector2i(static_cast<int>(tileGrid[0].size()),
                        static_cast<int>(tileGrid.size()));
}

sf::Vector2f RoomData::getRoomDimensions() const
{
    sf::Vector2i gridDims = getRoomGridDimensions();
    return sf::Vector2f(gridDims.x * tileSize,
                        gridDims.y * tileSize);
}

sf::Vector2f RoomData::getPlayerSpawn(const std::string &spawnKey,
                                      std::optional<sf::Vector2i> &passingOffset) const
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
                return resolveSpawn(e, passingOffset);
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
        if (e.type == Constants::ENTITY_ENEMY)
        {
            processEnemy(enemies, e);
        }
        else if (e.type == Constants::ENTITY_PLAYER_ABILITY)
        {
            processPlayerAbility(player, items, e);
        }
    }
}

// Privates

sf::Vector2f RoomData::resolveSpawn(const RoomEntity &e,
                                    std::optional<sf::Vector2i> &passingOffset) const
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

    // If no explicit spawnX/spawnY, apply offset based on exit direction
    if (passingOffset.has_value())
    {
        const std::string &dir = e.properties.at("exitDir");

        if (dir == "left" || dir == "right")
        {
            sy += passingOffset.value().y;
        }
        else if (dir == "up" || dir == "down")
        {
            sx += passingOffset.value().x;
        }
    }

    return {sx * tileSize,
            sy * tileSize};
}

void RoomData::processEnemy(
    std::vector<std::unique_ptr<Enemy>> &enemies,
    RoomEntity entity) const
{
    EnemyType enemyInt = static_cast<EnemyType>(std::stoi(entity.properties.at("type")));
    sf::Vector2f pos(entity.x * tileSize,
                     entity.y * tileSize);

    switch (enemyInt)
    {
    case EnemyType::SLUG:
    {
        int leftX = std::stoi(entity.properties.at("patrolLeftX"));
        int rightX = std::stoi(entity.properties.at("patrolRightX"));

        enemies.push_back(std::make_unique<SlugEnemy>(pos,
                                                      leftX * tileSize,
                                                      rightX * tileSize));
        break;
    }
    case EnemyType::CHARGER:
    {
        int leftX = std::stoi(entity.properties.at("patrolLeftX"));
        int rightX = std::stoi(entity.properties.at("patrolRightX"));

        enemies.push_back(std::make_unique<ChargerEnemy>(pos,
                                                         leftX * tileSize,
                                                         rightX * tileSize));
        break;
    }
    case EnemyType::FLIER:
    {
        int leftX = std::stoi(entity.properties.at("patrolLeftX"));
        int rightX = std::stoi(entity.properties.at("patrolRightX"));

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

        enemies.push_back(std::make_unique<SwimmerEnemy>(pos,
                                                         leftX * tileSize,
                                                         rightX * tileSize,
                                                         topY * tileSize,
                                                         bottomY * tileSize));
        break;
    }
    case EnemyType::CRAWLER:
    {
        enemies.push_back(std::make_unique<CrawlerEnemy>(pos));
        break;
    }
    default:
        std::cout << "Unknown EnemyType: "
                  << EnumUtils::enumToString(enemyInt)
                  << std::endl;
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