#include "constants/Constants.h"
#include "utils/GameUtils.h"
#include "MiniMap.h"

MiniMap::MiniMap(float uiWidth, float uiHeight)
    : uiWidth(uiWidth), uiHeight(uiHeight),
      rtSize(256),
      blackoutColour(Constants::uiBackgroundColour)
{
    background.setFillColor(blackoutColour);

    rt.create(rtSize, rtSize);
    rtSprite.setTexture(rt.getTexture());
}

void MiniMap::update(const sf::View &uiView, const GameData &gameData, const TileMap &tileMap)
{
    sf::Vector2i dims = gameData.getRoomData().getRoomGridDimensions();
    float roomW = dims.x * tileMap.tileSize;
    float roomH = dims.y * tileMap.tileSize;
    sf::Vector2f viewSize = uiView.getSize();

    // --- UI Scaling ---
    float scaleX = uiWidth / rtSize;
    float scaleY = uiHeight / rtSize;
    float padRight = 10.f;
    float padTop = 10.f;

    scale = std::min(scaleX, scaleY);
    rtSprite.setScale(scale, scale);

    float spriteWidth = rtSprite.getGlobalBounds().width;
    float xPos = viewSize.x - spriteWidth - padRight;
    rtSprite.setPosition({xPos, padTop});

    // Fixed View Size
    float mainViewWidthInTiles = Constants::VIEW_WIDTH / tileMap.tileSize;
    float zoomFactor = 2.0f;
    float viewW = mainViewWidthInTiles * zoomFactor * tileMap.tileSize;
    float viewH = viewW;

    sf::View v;
    v.setSize(viewW, viewH);

    // Center & Clamp Logic
    sf::Vector2f p = gameData.getPlayer()->getPosition();
    float halfW = viewW * 0.5f;
    float halfH = viewH * 0.5f;
    float cx, cy;

    // X-Axis
    if (roomW <= viewW)
    {
        cx = roomW / 2.f;
    }
    else
    {
        cx = std::clamp(p.x, halfW, roomW - halfW);
    }

    // Y-Axis
    if (roomH <= viewH)
    {
        cy = roomH / 2.f;
    }
    else
    {
        cy = std::clamp(p.y, halfH, roomH - halfH);
    }

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
