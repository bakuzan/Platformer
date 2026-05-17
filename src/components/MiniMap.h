#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "components/TileMap.h"
#include "core/GameData.h"

class MiniMap
{
public:
    MiniMap(float uiWidth, float uiHeight);

    void update(const GameData &gameData, const TileMap &tileMap);
    void render(sf::RenderWindow &window,
                const GameData &gameData,
                const TileMap &tileMap);

private:
    float uiWidth, uiHeight;

    sf::Color blackoutColour;
    sf::RectangleShape background;
    sf::RenderTexture rt; // minimap render target
    sf::Sprite rtSprite;  // sprite to draw in UI

    float scale;
    sf::Vector2f uiPos; // where to draw in UI

    void drawWorld(const TileMap &tileMap);
    void drawFog(const GameData &gameData, float tileSize);
    void drawEntities(const GameData &gameData, float tileSize);
};

#endif
