#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <unordered_map>

#include "data/RoomData.h"
#include "data/TileDefinition.h"

class TileMap
{
private:
    const std::unordered_map<char, TileDefinition> tileRegistry;
    std::vector<std::string> symbolGrid;
    std::vector<sf::RectangleShape> tiles;

public:
    const float tileSize = 32.0f;

public:
    TileMap(const std::unordered_map<char, TileDefinition> &registry);
    ~TileMap();

    void loadFromRoom(const RoomData &room);
    void render(sf::RenderWindow &window);

    TileProperties getTilePropertiesAtWorld(sf::Vector2f worldPos) const;
};

#endif // TILEMAP_H
