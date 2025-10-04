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
    tileVertices.setPrimitiveType(sf::Quads);
    tileVertices.clear();

    for (size_t y = 0; y < room.tileGrid.size(); ++y)
    {
        const std::string &row = room.tileGrid[y];
        for (size_t x = 0; x < row.size(); ++x)
        {
            char symbol = row[x];

            if (!tileRegistry.count(symbol))
            {
                continue;
            }

            const TileDefinition &def = tileRegistry.at(symbol);
            float px = x * tileSize;
            float py = y * tileSize;

            // Define the four corners of the tile quad
            sf::Vertex v0(sf::Vector2f(px, py), def.color);
            sf::Vertex v1(sf::Vector2f(px + tileSize, py), def.color);
            sf::Vertex v2(sf::Vector2f(px + tileSize, py + tileSize), def.color);
            sf::Vertex v3(sf::Vector2f(px, py + tileSize), def.color);

            tileVertices.append(v0);
            tileVertices.append(v1);
            tileVertices.append(v2);
            tileVertices.append(v3);
        }
    }
}

void TileMap::render(sf::RenderWindow &window)
{
    window.draw(tileVertices);
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
