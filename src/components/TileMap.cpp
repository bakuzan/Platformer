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

TileProperties TileMap::getTilePropertiesAtWorld(sf::Vector2f worldPos) const
{
    int x = static_cast<int>(worldPos.x / tileSize);
    int y = static_cast<int>(worldPos.y / tileSize);

    if (y < 0 || y >= symbolGrid.size() ||
        x < 0 || x >= symbolGrid[y].size())
    {
        throw std::out_of_range("TileMap: World position maps to invalid grid coordinates.");
    }

    char symbol = symbolGrid[y][x];
    auto it = tileRegistry.find(symbol);

    if (it == tileRegistry.end())
    {
        throw std::runtime_error("TileMap: No tile definition found for symbol '" + std::string(1, symbol) + "'");
    }

    return it->second.properties;
}

bool TileMap::isSolidTile(int gridX, int gridY) const
{
    if (gridY < 0 || gridY >= static_cast<int>(symbolGrid.size()) ||
        gridX < 0 || gridX >= static_cast<int>(symbolGrid[gridY].size()))
    {
        return false; // Treat out-of-bounds as non-solid
    }

    char symbol = symbolGrid[gridY][gridX];
    auto it = tileRegistry.find(symbol);

    if (it == tileRegistry.end())
    {
        return false;
    }

    return it->second.properties.isSolid;
}