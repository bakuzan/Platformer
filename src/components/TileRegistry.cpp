#include "TileRegistry.h"

#include "constants/Solidity.h"
#include "data/TileProperties.h"
#include "utils/GameUtils.h"

TileRegistry::TileRegistry()
{
    // Constructor
}

TileRegistry::~TileRegistry()
{
    // Destructor
}

// Publics

std::unordered_map<char, TileDefinition> TileRegistry::createTileRegistry()
{
    std::unordered_map<char, TileDefinition> registry;

    char VOID = GameUtils::getTileSymbol(TileName::VOID);
    char BASIC_GROUND = GameUtils::getTileSymbol(TileName::BASIC_GROUND);
    char WEAK_GROUND = GameUtils::getTileSymbol(TileName::WEAK_GROUND);
    char WATER = GameUtils::getTileSymbol(TileName::WATER);
    char PLATFORM = GameUtils::getTileSymbol(TileName::PLATFORM);
    char SCENERY = GameUtils::getTileSymbol(TileName::SCENERY);
    char PILLAR = GameUtils::getTileSymbol(TileName::PILLAR);
    char LAVA = GameUtils::getTileSymbol(TileName::LAVA);
    char CLIMBABLE = GameUtils::getTileSymbol(TileName::CLIMBABLE);

    registry[BASIC_GROUND] = {sf::Color(128, 90, 0),
                              TileProperties::createSolidTileProperties()};
    registry[WEAK_GROUND] = {sf::Color(97, 63, 0),
                             TileProperties::createSolidTileProperties(true)};
    registry[VOID] = {sf::Color::Black,
                      TileProperties::makeEmpty()};

    registry[PILLAR] = {sf::Color(162, 157, 157),
                        TileProperties::createSolidTileProperties()};
    registry[PLATFORM] = {sf::Color(66, 66, 66),
                          TileProperties::createTileProperties(TileCategory::PLATFORM, Solidity::TOP)};
    registry[SCENERY] = {sf::Color(134, 134, 134),
                         TileProperties::createTileProperties(TileCategory::BACKGROUND, Solidity::NONE)};

    registry[WATER] = {sf::Color::Blue,
                       TileProperties::createWaterTileProperties()};
    registry[LAVA] = {sf::Color(255, 69, 0),
                      TileProperties::createWaterTileProperties(5)};
    registry[CLIMBABLE] = {sf::Color(50, 255, 50),
                           TileProperties::createSolidTileProperties(false, true)};

    return registry;
}
