#include "constants/Constants.h"
#include "utils/GameUtils.h"
#include "MiniMap.h"

MiniMap::MiniMap(float uiWidth, float uiHeight)
    : uiWidth(uiWidth), uiHeight(uiHeight),
      blackoutColour(Constants::uiBackgroundColour)
{
    rt.create(256, 256); // square minimap buffer
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

    // View
    sf::View v;
    v.setCenter(roomW * 0.5f, roomH * 0.5f);

    float roomAspect = roomW / roomH;
    float rtAspect = 1.f; // 256x256

    if (roomAspect > rtAspect)
    {
        // room is wider → expand vertical view
        v.setSize(roomW, roomW);
    }
    else
    {
        // room is taller → expand horizontal view
        v.setSize(roomH, roomH);
    }

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
    rt.clear(blackoutColour);

    float ts = tileMap.tileSize;

    drawWorld(tileMap);
    drawEntities(gameData, ts);
    drawFog(gameData, ts);

    rt.display();

    window.draw(rtSprite);
}
