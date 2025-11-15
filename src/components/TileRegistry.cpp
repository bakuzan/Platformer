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

    registry[BASIC_GROUND] = {sf::Color(128, 90, 0),
                              TileProperties::createTileProperties(TileCategory::SOLID, Solidity::BOTH, false)};
    registry[WEAK_GROUND] = {sf::Color(97, 63, 0),
                             TileProperties::createTileProperties(TileCategory::SOLID, Solidity::BOTH, true)};
    registry[VOID] = {sf::Color::Black,
                      TileProperties::createTileProperties(TileCategory::EMPTY, Solidity::NONE, false)};

    registry[PLATFORM] = {sf::Color(66, 66, 66),
                          TileProperties::createTileProperties(TileCategory::PLATFORM, Solidity::TOP, false)};
    registry[SCENERY] = {sf::Color(134, 134, 134),
                         TileProperties::createTileProperties(TileCategory::BACKGROUND, Solidity::NONE, false)};

    registry[WATER] = {sf::Color::Blue,
                       TileProperties::createTileProperties(TileCategory::WATER, Solidity::NONE, false)};

    return registry;
}
