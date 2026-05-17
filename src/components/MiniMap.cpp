#include "constants/Constants.h"
#include "utils/GameUtils.h"
#include "MiniMap.h"

MiniMap::MiniMap(float uiWidth, float uiHeight)
    : uiWidth(uiWidth), uiHeight(uiHeight),
      blackoutColour(Constants::uiBackgroundColour)
{
    background.setFillColor(blackoutColour);

    rt.create(256, 256);
    rtSprite.setTexture(rt.getTexture());
}

void MiniMap::update(const GameData &gameData, const TileMap &tileMap)
{
    sf::Vector2i dims = gameData.getRoomData().getRoomGridDimensions();
    float roomW = dims.x * tileMap.tileSize;
    float roomH = dims.y * tileMap.tileSize;

    float rtSize = 256.f;

    // UI scaling
    float scaleX = uiWidth / rtSize;
    float scaleY = uiHeight / rtSize;
    scale = std::min(scaleX, scaleY);
    rtSprite.setScale(scale, scale);
    rtSprite.setPosition({1920 - uiWidth - 10.f, 10.f});

    // dynamic zoom based on room size
    float targetTiles = 22.f;
    float roomTilesX = static_cast<float>(dims.x);
    float roomTilesY = static_cast<float>(dims.y);

    float visibleTilesX = std::min(targetTiles, roomTilesX);
    float visibleTilesY = std::min(targetTiles, roomTilesY);

    float viewW = visibleTilesX * tileMap.tileSize;
    float viewH = visibleTilesY * tileMap.tileSize;

    sf::View v;
    v.setSize(viewW, viewH);

    // center on player
    sf::Vector2f p = gameData.getPlayer()->getPosition();
    v.setCenter(p);

    // clamp to room bounds
    float halfW = viewW * 0.5f;
    float halfH = viewH * 0.5f;

    float cx = std::clamp(p.x, halfW, roomW - halfW);
    float cy = std::clamp(p.y, halfH, roomH - halfH);
    v.setCenter(cx, cy);

    rt.setView(v);
}

void MiniMap::drawWorld(const TileMap &tileMap)
{
    tileMap.render(rt);
}

void MiniMap::drawEntities(const GameData &gameData, float tileSize)
{
    // items
    for (const auto &itemPtr : gameData.getItems())
    {
        sf::Sprite item = itemPtr->getSprite();
        rt.draw(item);
    }

    // savepoints
    for (const auto &sp : gameData.getRoomData().savePoints)
    {
        sf::FloatRect r = GameUtils::getRectForRoomEntity(sp, tileSize);

        sf::RectangleShape safePoint({r.width, r.height});
        safePoint.setFillColor(Constants::savepointColour);
        safePoint.setPosition(r.left, r.top);
        rt.draw(safePoint);
    }

    // player
    const Player &p = *gameData.getPlayer();
    sf::Sprite player = p.getSprite();
    rt.draw(player);
}

void MiniMap::drawFog(const GameData &gameData, float tileSize)
{
    const auto &revealed = gameData.getRevealedRoomTiles(gameData.getRoomData().roomId);

    sf::RectangleShape fog({tileSize, tileSize});
    fog.setFillColor(blackoutColour);

    for (int y = 0; y < revealed.size(); y++)
    {
        for (int x = 0; x < revealed[y].size(); x++)
        {
            if (!revealed[y][x])
            {
                fog.setPosition(x * tileSize, y * tileSize);
                rt.draw(fog);
            }
        }
    }
}

void MiniMap::render(sf::RenderWindow &window,
                     const GameData &gameData,
                     const TileMap &tileMap)
{
    rt.clear(sf::Color::Transparent);

    float ts = tileMap.tileSize;

    drawWorld(tileMap);
    drawEntities(gameData, ts);
    drawFog(gameData, ts);

    rt.display();

    sf::FloatRect bounds = rtSprite.getGlobalBounds();
    float pad = 3.f;
    background.setSize({bounds.width + pad * 2.f, bounds.height + pad * 2.f});
    background.setPosition(bounds.left - pad, bounds.top - pad);

    window.draw(background);
    window.draw(rtSprite);
}
