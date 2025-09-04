#include "TileRegistry.h"

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
    registry['#'] = {sf::Color::Magenta, {TileType::SOLID, false, false, false}};
    registry['.'] = {sf::Color::Transparent, {TileType::EMPTY, true, false, false}};
    registry['B'] = {sf::Color::Blue, {TileType::WATER, true, true, false}};

    return registry;
}
