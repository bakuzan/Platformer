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
    registry['#'] = {sf::Color::Magenta,
                     TileProperties::createTileProperties(TileType::SOLID, Solidity::BOTH, false, false)};
    registry['.'] = {sf::Color(165, 42, 42),
                     TileProperties::createTileProperties(TileType::EMPTY, Solidity::NONE, false, false)};

    registry['-'] = {sf::Color(35, 145, 9),
                     TileProperties::createTileProperties(TileType::PLATFORM, Solidity::TOP, false, false)};
    registry['='] = {sf::Color::Green,
                     TileProperties::createTileProperties(TileType::BACKGROUND, Solidity::NONE, false, false)};

    return registry;
}
