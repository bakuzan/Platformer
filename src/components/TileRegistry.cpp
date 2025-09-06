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
    registry['#'] = {sf::Color::Magenta, {TileType::SOLID, true, false, false}};
    registry['.'] = {sf::Color(165, 42, 42), {TileType::EMPTY, false, false, false}};
    registry['B'] = {sf::Color::Blue, {TileType::WATER, false, true, false}};

    return registry;
}
