#include "TileMap.h"

TileMap::TileMap(const std::unordered_map<char, TileDefinition> &registry)
    : tileRegistry(registry)
{
    // Constructor
}

TileMap::~TileMap()
{
    // Destructor
}

// Publics

void TileMap::loadFromRoom(const RoomData &room)
{
    symbolGrid = room.tileGrid;
    tiles.clear();

    for (size_t y = 0; y < room.tileGrid.size(); ++y)
    {
        const std::string &row = room.tileGrid[y];

        for (size_t x = 0; x < row.size(); ++x)
        {
            char symbol = row[x];

            if (tileRegistry.count(symbol))
            {
                const TileDefinition &def = tileRegistry.at(symbol);

                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(x * tileSize, y * tileSize);
                tile.setFillColor(def.color);

                tiles.push_back(tile);
            }
        }
    }
}

void TileMap::render(sf::RenderWindow &window)
{
    for (const auto &tile : tiles)
    {
        window.draw(tile);
    }
}

std::optional<TileProperties> TileMap::getTilePropertiesAtTile(int tileX, int tileY) const
{
    if (tileY < 0 || tileY >= static_cast<int>(symbolGrid.size()) ||
        tileX < 0 || tileX >= static_cast<int>(symbolGrid[tileY].size()))
    {
        return std::nullopt;
    }

    char symbol = symbolGrid[tileY][tileX];
    auto it = tileRegistry.find(symbol);

    if (it == tileRegistry.end())
    {
        throw std::runtime_error(
            "TileMap: No tile definition found for symbol '" + std::string(1, symbol) + "'");
    }

    return it->second.properties;
}
