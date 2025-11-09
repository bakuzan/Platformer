#include "TileRegistry.h"

#include "constants/Solidity.h"
#include "data/TileProperties.h"

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
    // TODO Update to reflect the real tiles using sprites
    registry['#'] = {sf::Color(128, 90, 0),
                     TileProperties::createTileProperties(TileType::SOLID, Solidity::BOTH)};
    registry['@'] = {sf::Color(97, 63, 0),
                     TileProperties::createTileProperties(TileType::SOLID, Solidity::BOTH, true)};
    registry['.'] = {sf::Color::Black,
                     TileProperties::createTileProperties(TileType::EMPTY, Solidity::NONE)};

    registry['-'] = {sf::Color(66, 66, 66),
                     TileProperties::createTileProperties(TileType::PLATFORM, Solidity::TOP)};
    registry['='] = {sf::Color(134, 134, 134),
                     TileProperties::createTileProperties(TileType::BACKGROUND, Solidity::NONE)};

    registry['~'] = {sf::Color::Blue,
                     TileProperties::createTileProperties(TileType::WATER, Solidity::NONE)};

    return registry;
}
