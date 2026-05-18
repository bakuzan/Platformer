#include <limits>

#include "constants/Constants.h"
#include "utils/GameUtils.h"
#include "LevelMap.h"

LevelMap::LevelMap(const std::unordered_map<char, TileDefinition> &registry,
                   GameData &gd)
    : tileMap(registry), gameData(gd),
      blackoutColour(Constants::uiBackgroundColour),
      savepointColour(Constants::savepointColour),
      itemColour(Constants::playerAbilityColour)
{
    background.setFillColor(blackoutColour);
}

LevelMap::~LevelMap()
{
    // Destructor
}

// Publics

void LevelMap::prepare(const std::vector<std::string> &levelRooms,
                       const std::string &startRoomId)
{
    roomDataMap = loadLevelRoomData(levelRooms);
    if (roomDataMap.empty())
    {
        ready = false;
        return;
    }

    auto offsets = computeRoomOffsets(startRoomId);
    buildPlacedRoomsFromOffsets(offsets);
    buildTileMap();
    syncRevealedFromGameData();

    // View setup
    float mapW = static_cast<float>(levelSizeTiles.x) * tileMap.tileSize;
    float mapH = static_cast<float>(levelSizeTiles.y) * tileMap.tileSize;
    view.setSize(mapW, mapH);
    view.setCenter(mapW * 0.5f, mapH * 0.5f);

    ready = true;
}

void LevelMap::handleZoom(const sf::RenderWindow &window, float wheelDelta)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f before = window.mapPixelToCoords(pixelPos, view);

    // Apply zoom factor
    float factor = (wheelDelta > 0) ? 0.9f : 1.1f;
    zoomLevel *= factor;
    zoomLevel = std::max(zoomMin, std::min(zoomMax, zoomLevel));
    view.zoom(factor);

    // convert to world coords AFTER zoom
    sf::Vector2f after = window.mapPixelToCoords(pixelPos, view);

    // move view so the point under the mouse stays fixed
    view.move(before - after);
}

void LevelMap::render(sf::RenderWindow &window,
                      const sf::FloatRect &viewport)
{
    if (!ready)
    {
        return;
    }

    sf::Vector2u winSize = window.getSize();
    sf::View uiView(sf::FloatRect(0.f, 0.f,
                                  static_cast<float>(winSize.x),
                                  static_cast<float>(winSize.y)));
    window.setView(uiView);

    float px = viewport.left * winSize.x;
    float py = viewport.top * winSize.y;
    float pw = viewport.width * winSize.x;
    float ph = viewport.height * winSize.y;

    background.setOrigin(0.f, 0.f);
    background.setPosition(px, py);
    background.setSize({pw, ph});

    window.draw(background);

    // Map
    view.setViewport(viewport);
    window.setView(view);

    tileMap.render(window);
    renderEntities(window);
    renderFog(window);
}

void LevelMap::renderFog(sf::RenderWindow &window)
{
    if (!ready)
    {
        return;
    }

    float ts = tileMap.tileSize;
    sf::RectangleShape fogTile({ts, ts});
    fogTile.setFillColor(blackoutColour);

    int h = static_cast<int>(revealedGrid.size());
    int w = (h > 0) ? static_cast<int>(revealedGrid[0].size()) : 0;

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            if (!revealedGrid[y][x])
            {
                fogTile.setPosition(static_cast<float>(x) * ts, static_cast<float>(y) * ts);
                window.draw(fogTile);
            }
        }
    }
}

bool LevelMap::isReady() const
{
    return ready;
}

// Privates

std::unordered_map<std::string, RoomData> LevelMap::loadLevelRoomData(
    const std::vector<std::string> &levelRooms)
{
    std::unordered_map<std::string, RoomData> map;

    for (const auto &rid : levelRooms)
    {
        RoomData rd = RoomLoader::loadFromId(rid, tileMap.tileSize);
        map.emplace(rid, std::move(rd));
    }

    return map;
}

std::unordered_map<std::string, sf::Vector2i> LevelMap::computeRoomOffsets(
    const std::string &startRoomId)
{
    std::unordered_map<std::string, sf::Vector2i> offsets;
    if (roomDataMap.find(startRoomId) == roomDataMap.end())
    {
        return offsets;
    }

    offsets[startRoomId] = {0, 0};

    bool changed = true;
    int pass = 0;
    int maxPasses = static_cast<int>(roomDataMap.size()) + 2;

    while (changed &&
           pass < maxPasses)
    {
        changed = false;
        ++pass;

        for (const auto &pair : roomDataMap)
        {
            const std::string &aId = pair.first;
            const RoomData &aRoom = pair.second;

            if (offsets.find(aId) == offsets.end())
            {
                continue;
            }

            sf::Vector2i aOff = offsets[aId];

            for (const auto &entr : aRoom.entrances)
            {
                auto itTarget = entr.properties.find("target");
                if (itTarget == entr.properties.end())
                {
                    continue;
                }

                const std::string bId = itTarget->second;
                if (roomDataMap.find(bId) == roomDataMap.end())
                {
                    continue;
                }

                if (offsets.find(bId) != offsets.end())
                {
                    continue;
                }

                const RoomData &bRoom = roomDataMap.at(bId);
                bool matched = false;

                for (const auto &bEntr : bRoom.entrances)
                {
                    auto itBack = bEntr.properties.find("target");
                    if (itBack == bEntr.properties.end())
                    {
                        continue;
                    }

                    if (itBack->second != aId)
                    {
                        continue;
                    }

                    offsets[bId] = aOff + sf::Vector2i(entr.x - bEntr.x, entr.y - bEntr.y);
                    changed = true;
                    matched = true;
                    break;
                }

                if (!matched)
                {
                    offsets[bId] = aOff + sf::Vector2i(entr.x, entr.y);
                    changed = true;
                }
            }
        }
    }

    return offsets;
}

void LevelMap::buildPlacedRoomsFromOffsets(
    const std::unordered_map<std::string, sf::Vector2i> &offsets)
{
    placedRooms.clear();

    // Build list in level order if you want stable ordering; otherwise iterate map
    for (const auto &pair : roomDataMap)
    {
        const std::string &rid = pair.first;
        const RoomData &rd = pair.second;
        sf::Vector2i off = {0, 0};

        if (offsets.find(rid) != offsets.end())
        {
            off = offsets.at(rid);
        }

        placedRooms.push_back({rd, off});
    }

    // compute bounds
    int minX = INT_MAX, minY = INT_MAX,
        maxX = INT_MIN, maxY = INT_MIN;

    for (auto &pr : placedRooms)
    {
        int rx = pr.offsetTiles.x;
        int ry = pr.offsetTiles.y;
        int w = pr.room.getRoomGridDimensions().x;
        int h = pr.room.getRoomGridDimensions().y;

        minX = std::min(minX, rx);
        minY = std::min(minY, ry);
        maxX = std::max(maxX, rx + w);
        maxY = std::max(maxY, ry + h);
    }

    if (minX == INT_MAX)
    {
        levelSizeTiles = {0, 0};
        return;
    }

    for (auto &pr : placedRooms)
    {
        pr.offsetTiles -= sf::Vector2i(minX, minY);
    }

    levelSizeTiles = {maxX - minX, maxY - minY};
}

void LevelMap::buildTileMap()
{
    if (levelSizeTiles.x <= 0 ||
        levelSizeTiles.y <= 0)
    {
        ready = false;
        return;
    }

    tileMap.initEmpty(levelSizeTiles.x, levelSizeTiles.y);

    for (const auto &pr : placedRooms)
    {
        const RoomData &r = pr.room;
        sf::Vector2i dims = r.getRoomGridDimensions();
        sf::Vector2i off = pr.offsetTiles;

        for (int y = 0; y < dims.y; ++y)
        {
            for (int x = 0; x < dims.x; ++x)
            {
                tileMap.setSymbolAt(x + off.x, y + off.y, r.tileGrid[y][x]);
            }
        }

        // Apply destroyed tiles
        auto destroyed = gameData.getDestroyedRoomTiles(r.roomId);
        for (const auto &tk : destroyed)
        {
            tileMap.makeTileVoid(tk.x + off.x, tk.y + off.y);
        }
    }

    tileMap.rebuildVertices();
}

void LevelMap::syncRevealedFromGameData()
{
    if (levelSizeTiles.x <= 0 ||
        levelSizeTiles.y <= 0)
    {
        revealedGrid.clear();
        return;
    }

    revealedGrid.assign(levelSizeTiles.y, std::vector<bool>(levelSizeTiles.x, false));

    for (const auto &pr : placedRooms)
    {
        const std::string &rid = pr.room.roomId;
        const auto &roomRevealed = gameData.getRevealedRoomTiles(rid);

        if (roomRevealed.empty())
        {
            continue;
        }

        sf::Vector2i dims = pr.room.getRoomGridDimensions();
        int maxRy = std::min<int>(dims.y, static_cast<int>(roomRevealed.size()));
        for (int ry = 0; ry < maxRy; ++ry)
        {
            int maxRx = std::min<int>(dims.x, static_cast<int>(roomRevealed[ry].size()));
            for (int rx = 0; rx < maxRx; ++rx)
            {
                if (!roomRevealed[ry][rx])
                {
                    continue;
                }

                int gx = pr.offsetTiles.x + rx;
                int gy = pr.offsetTiles.y + ry;
                if (gy >= 0 && gy < levelSizeTiles.y &&
                    gx >= 0 && gx < levelSizeTiles.x)
                {
                    revealedGrid[gy][gx] = true;
                }
            }
        }
    }
}

void LevelMap::renderEntities(sf::RenderWindow &window)
{
    if (!ready)
    {
        return;
    }

    float ts = tileMap.tileSize;
    auto player = gameData.getPlayer();
    auto currentRoomData = gameData.getRoomData();
    sf::Sprite s = player->getSprite();
    sf::Vector2f reported = player->getPosition();

    for (const auto &pr : placedRooms)
    {
        const RoomData &rd = pr.room;

        for (const auto &sp : rd.savePoints)
        {
            sf::FloatRect rect = GameUtils::getRectForRoomEntity(sp, ts);
            sf::RectangleShape safePoint(sf::Vector2f(rect.width, rect.height));
            safePoint.setFillColor(savepointColour);
            safePoint.setPosition(pr.offsetTiles.x * ts + rect.left,
                                  pr.offsetTiles.y * ts + rect.top);
            window.draw(safePoint);
        }

        for (const auto &it : rd.entities)
        {
            if (it.type == Constants::ENTITY_PLAYER_ABILITY)
            {
                sf::FloatRect rect = GameUtils::getRectForRoomEntity(it, ts);
                sf::RectangleShape itemShape(sf::Vector2f(rect.width, rect.height));
                itemShape.setFillColor(itemColour);
                itemShape.setPosition(pr.offsetTiles.x * ts + rect.left,
                                      pr.offsetTiles.y * ts + rect.top);
                window.draw(itemShape);
            }
        }

        if (currentRoomData.roomId == pr.room.roomId)
        {
            // simple tile-vs-pixel check using this room's dims
            auto dims = pr.room.getRoomGridDimensions();
            bool looksLikeTiles = (reported.x >= 0 && reported.y >= 0 &&
                                   reported.x < dims.x && reported.y < dims.y &&
                                   std::floor(reported.x) == reported.x && std::floor(reported.y) == reported.y);

            sf::Vector2f localPx = looksLikeTiles
                                       ? sf::Vector2f(reported.x * ts, reported.y * ts)
                                       : reported;

            // place using the known room offset
            s.setPosition(pr.offsetTiles.x * ts + localPx.x,
                          pr.offsetTiles.y * ts + localPx.y);
            window.draw(s);
        }
    }
}
