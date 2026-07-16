#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>

#include <optional>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "data/TileKey.h"
#include "data/RoomData.h"
#include "data/TileDefinition.h"

class TileMap
{
private:
    const std::unordered_map<char, TileDefinition> tileRegistry;
    std::vector<std::string> symbolGrid;
    sf::VertexArray tileVertices;

public:
    const float tileSize = 32.0f;

private:
    void redrawTileVertices(int tileX, int tileY, const char &tileSymbol);
    void applyPlatformisation(const RoomData &room,
                              const std::unordered_set<TileKey, TileKeyHash> &destroyedTiles);

public:
    TileMap(const std::unordered_map<char, TileDefinition> &registry);
    ~TileMap();

    void loadFromRoom(const RoomData &room,
                      const std::unordered_set<TileKey, TileKeyHash> &destroyedTiles);
    void render(sf::RenderTarget &target) const;

    int getHeight() const;
    int getWidth() const;
    float getHeightInPixels() const;
    float getWidthInPixels() const;

    void initEmpty(int widthTiles, int heightTiles);
    void setSymbolAt(int tileX, int tileY, char symbol);
    void rebuildVertices();
    const std::unordered_map<char, TileDefinition> &getRegistry() const;

    std::optional<TileProperties> getTilePropertiesAtTile(int tileX, int tileY) const;
    void makeTileVoid(int tileX, int tileY);

    bool hasLineOfSight(const sf::Vector2f &start, const sf::Vector2f &end) const;
};

#endif // TILEMAP_H
