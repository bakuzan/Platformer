#include "TileMap.h"
#include "utils/GameUtils.h"

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

void TileMap::loadFromRoom(const RoomData &room,
                           const std::unordered_set<TileKey, TileKeyHash> &destroyedTiles)
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

            auto destroyedIt = destroyedTiles.find({static_cast<int>(x), static_cast<int>(y)});
            if (destroyedIt != destroyedTiles.end())
            {
                symbol = GameUtils::getTileSymbol(TileName::VOID);
                symbolGrid[y][x] = symbol;
            }

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

void TileMap::render(sf::RenderTarget &target) const
{
    target.draw(tileVertices);
}

int TileMap::getHeight() const
{
    return static_cast<int>(symbolGrid.size());
}

int TileMap::getWidth() const
{
    return static_cast<int>(symbolGrid[0].size());
}

float TileMap::getHeightInPixels() const
{
    return getHeight() * tileSize;
}

float TileMap::getWidthInPixels() const
{
    return getWidth() * tileSize;
}

void TileMap::initEmpty(int widthTiles, int heightTiles)
{
    symbolGrid.assign(static_cast<size_t>(heightTiles),
                      std::string(static_cast<size_t>(widthTiles),
                                  GameUtils::getTileSymbol(TileName::VOID)));

    tileVertices.clear();
    tileVertices.setPrimitiveType(sf::Quads);
}

void TileMap::setSymbolAt(int tileX, int tileY, char symbol)
{
    if (tileY < 0 ||
        tileY >= static_cast<int>(symbolGrid.size()))
    {
        return;
    }

    if (tileX < 0 ||
        tileX >= static_cast<int>(symbolGrid[tileY].size()))
    {
        return;
    }

    symbolGrid[tileY][tileX] = symbol;
}

void TileMap::rebuildVertices()
{
    tileVertices.clear();
    tileVertices.setPrimitiveType(sf::Quads);

    for (size_t y = 0; y < symbolGrid.size(); ++y)
    {
        const std::string &row = symbolGrid[y];

        for (size_t x = 0; x < row.size(); ++x)
        {
            char symbol = row[x];
            auto it = tileRegistry.find(symbol);
            if (it == tileRegistry.end())
            {
                continue;
            }

            const TileDefinition &def = it->second;
            float px = static_cast<float>(x) * tileSize;
            float py = static_cast<float>(y) * tileSize;

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

const std::unordered_map<char, TileDefinition> &TileMap::getRegistry() const
{
    return tileRegistry;
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

void TileMap::makeTileVoid(int tileX, int tileY)
{
    if (tileY < 0 || tileY >= static_cast<int>(symbolGrid.size()) ||
        tileX < 0 || tileX >= static_cast<int>(symbolGrid[tileY].size()))
    {
        return;
    }

    // Force tile to be as a "background" tile.
    char tileSymbol = GameUtils::getTileSymbol(TileName::VOID);
    symbolGrid[tileY][tileX] = tileSymbol;
    redrawTileVertices(tileX, tileY, tileSymbol);
}

bool TileMap::hasLineOfSight(const sf::Vector2f &start, const sf::Vector2f &end) const
{
    float distance = GameUtils::getDistanceBetween(start, end);
    if (distance == 0.f)
    {
        return true;
    }

    sf::Vector2f diff = end - start;
    sf::Vector2f direction = diff / distance;

    float stepSize = tileSize * 0.5f;
    int totalSteps = static_cast<int>(distance / stepSize);

    for (int i = 0; i <= totalSteps; ++i)
    {
        sf::Vector2f currentPos = start + (direction * (stepSize * i));

        int tileX = static_cast<int>(currentPos.x / tileSize);
        int tileY = static_cast<int>(currentPos.y / tileSize);

        auto props = getTilePropertiesAtTile(tileX, tileY);

        if (props && props->solidity == Solidity::BOTH)
        {
            return false;
        }
    }

    return true;
}

// Privates

void TileMap::redrawTileVertices(int tileX, int tileY, const char &tileSymbol)
{
    std::size_t quadIndex = (tileY * symbolGrid[0].size() + tileX) * 4;

    if (quadIndex + 3 < tileVertices.getVertexCount())
    {
        float px = tileX * tileSize;
        float py = tileY * tileSize;

        const TileDefinition &def = tileRegistry.at(tileSymbol);

        tileVertices[quadIndex + 0].position = {px, py};
        tileVertices[quadIndex + 0].color = def.color;

        tileVertices[quadIndex + 1].position = {px + tileSize, py};
        tileVertices[quadIndex + 1].color = def.color;

        tileVertices[quadIndex + 2].position = {px + tileSize, py + tileSize};
        tileVertices[quadIndex + 2].color = def.color;

        tileVertices[quadIndex + 3].position = {px, py + tileSize};
        tileVertices[quadIndex + 3].color = def.color;
    }
}
