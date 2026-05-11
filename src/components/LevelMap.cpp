#include <limits>

#include "LevelMap.h"

LevelMap::LevelMap(const std::unordered_map<char, TileDefinition> &registry,
                   GameData &gd)
    : tileMap(registry), gameData(gd)
{
    // Constructor
}

LevelMap::~LevelMap()
{
    // Destructor
}

// Publics

void LevelMap::prepare(const std::vector<std::string> &levelRooms,
                       const std::string &startRoomId)
{
    auto roomDataMap = loadLevelRoomData(levelRooms);
    if (roomDataMap.empty())
    {
        ready = false;
        return;
    }

    auto offsets = computeRoomOffsets(roomDataMap, startRoomId);
    buildPlacedRoomsFromOffsets(roomDataMap, offsets);
    buildTileMap();
}

void LevelMap::render(sf::RenderWindow &window,
                      const sf::FloatRect &viewport)
{
    if (!ready)
    {
        return;
    }

    sf::View mapView;
    float mapW = static_cast<float>(levelSizeTiles.x) * tileMap.tileSize;
    float mapH = static_cast<float>(levelSizeTiles.y) * tileMap.tileSize;

    mapView.setSize(mapW, mapH);
    mapView.setCenter(mapW / 2.f, mapH / 2.f);
    mapView.setViewport(viewport);
    window.setView(mapView);
    tileMap.render(window);
}

bool LevelMap::isReady() const
{
    return ready;
}

sf::Vector2i LevelMap::getLevelSizeTiles() const
{
    return levelSizeTiles;
}

const std::vector<RoomInstance> &LevelMap::getPlacedRooms() const
{
    return placedRooms;
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
    const std::unordered_map<std::string, RoomData> &roomDataMap,
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
    const std::unordered_map<std::string, RoomData> &roomDataMap,
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
    ready = true;
}
