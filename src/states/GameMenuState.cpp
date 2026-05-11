#include <memory>
#include <format>
#include <queue>

#include "constants/Constants.h"
#include "constants/PlayerAbility.h"
#include "components/RoomLoader.h"
#include "utils/EnumUtils.h"
#include "utils/InputUtils.h"
#include "LoadMenuState.h"
#include "MainMenuState.h"
#include "GameMenuState.h"

GameMenuState::GameMenuState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win),
      pauseTileMap(tileRegistry.createTileRegistry())
{
    buttonSpacing = Constants::BUTTON_HEIGHT + 10.f;
    sf::Vector2f center(pauseView.getCenter());
    window.setView(pauseView);

    // Configure background
    background.setSize(sf::Vector2f(300.f, 300.f));
    background.setFillColor(sf::Color(0, 0, 0, 255));

    // Configure text
    pauseText.setFont(gameData.gameFont);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(36);
    pauseText.setFillColor(sf::Color::White);

    // Add buttons
    addButton("Resume", "Resume",
              [this]()
              { stateManager.popState(); });
    addButton("Load", "Load Game",
              [this]()
              { stateManager.pushState(std::make_unique<LoadMenuState>(gameData, stateManager, window)); });
    addButton("Quit", "Quit Game",
              [this]()
              { gameData.reset();
                stateManager.replaceStates(std::make_unique<MainMenuState>(gameData, stateManager, window)); });
    addButton("Exit", "Exit",
              [this]()
              { window.close(); });

    // To ensure positioning is updated relative to window resizing
    updateMenuItemPositions();
}

GameMenuState::~GameMenuState()
{
    // Destructor
}

// Publics

void GameMenuState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed &&
        InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Escape, sf::Keyboard::P}))
    {
        stateManager.popState(); // Resume the game by returning to GameState
    }

    InputUtils::handleButtonEvent(event, buttons, window, selectedButtonIndex);
}

void GameMenuState::handleWindowResize(sf::Vector2u newSize)
{
    (void)newSize;
    updateMenuItemPositions();
}

void GameMenuState::update(sf::Time deltaTime)
{
    (void)deltaTime;

    if (!pauseMapReady)
    {
        preparePauseMap();
    }

    gameData.audioManager.cleanupSounds();
}

void GameMenuState::render()
{
    // UI
    window.setView(pauseView);
    window.draw(background);
    window.draw(pauseText);

    for (const auto &button : buttons)
    {
        button.render(window);
    }

    renderAbilitiesPanel(*gameData.getPlayer());

    // Map
    if (pauseMapReady)
    {
        sf::View mapView;
        float mapW = pauseLevelSizeTiles.x * pauseTileMap.tileSize;
        float mapH = pauseLevelSizeTiles.y * pauseTileMap.tileSize;

        mapView.setSize(mapW, mapH);
        mapView.setCenter(mapW / 2.f, mapH / 2.f);
        mapView.setViewport(sf::FloatRect(0.25f, 0.0f, 0.75f, 1.0f));
        window.setView(mapView);

        pauseTileMap.render(window);
        window.setView(pauseView);
    }

    // Reset view again
    window.setView(pauseView);
}

// Privates

void GameMenuState::addButton(std::string id,
                              std::string label,
                              std::function<void()> callback)
{
    buttons.emplace_back(id, gameData.gameFont, label, sf::Vector2f(0.f, 0.f), callback);
}

void GameMenuState::updateMenuItemPositions()
{
    sf::Vector2f viewSize = pauseView.getSize();

    float sidebarWidth = viewSize.x * 0.25f;
    background.setSize(sf::Vector2f(sidebarWidth, viewSize.y));
    background.setPosition(0, 0);

    pauseText.setPosition(
        (sidebarWidth - pauseText.getGlobalBounds().width) / 2.f,
        50.f);

    if (buttons.empty())
    {
        return;
    }

    float buttonX = (sidebarWidth - Constants::BUTTON_WIDTH) / 2.f;
    float startY = 150.f; // Starting Y after the title

    for (size_t i = 0; i < buttons.size(); ++i)
    {
        buttons[i].setPosition(sf::Vector2f(buttonX, startY + (i * buttonSpacing)));
    }
}

void GameMenuState::renderAbilitiesPanel(const Player &player)
{
    sf::Vector2f viewSize = pauseView.getSize();
    float sidebarWidth = viewSize.x * 0.25f;

    // Find bottom of last button
    float y = 0.f;
    if (!buttons.empty())
    {
        const auto &last = buttons.back();
        y = last.getPosition().y + Constants::BUTTON_HEIGHT + 40.f;
    }
    else
    {
        y = 200.f; // fallback if no buttons exist
    }

    // Header
    sf::Text header("Abilities", gameData.gameFont, 24);
    header.setFillColor(sf::Color::White);
    header.setPosition((sidebarWidth - header.getGlobalBounds().width) / 2.f, y);
    window.draw(header);

    y += 40.f;

    const auto &abilities = player.getCurrentAbilities();

    if (abilities.empty())
    {
        sf::Text none("- None -", gameData.gameFont, 20);
        none.setFillColor(sf::Color(180, 180, 180));
        none.setPosition((sidebarWidth - none.getGlobalBounds().width) / 2.f, y);
        window.draw(none);
        return;
    }

    for (auto ability : abilities)
    {
        // Parts
        sf::RectangleShape box({12.f, 12.f});
        box.setFillColor(Constants::playerAbility);
        sf::Text text(EnumUtils::enumToString(ability), gameData.gameFont, 20);
        text.setFillColor(sf::Color::White);

        // Placements
        float rowX = 5.f;
        box.setPosition(rowX, y + 4.f);
        window.draw(box);
        text.setPosition(rowX + 20.f, y);
        window.draw(text);

        // Padding
        y += 28.f;
    }
}

// Map methods
void GameMenuState::assemblePlacedRooms(const std::vector<std::string> &levelRooms,
                                        const std::string &startRoomId)
{
    auto roomDataMap = loadLevelRoomData(levelRooms);
    if (roomDataMap.empty())
    {
        pauseLevelSizeTiles = {0, 0};
        return;
    }

    auto offsets = computeRoomOffsets(roomDataMap, startRoomId);
    buildPlacedRoomsFromOffsets(roomDataMap, offsets);
}

std::unordered_map<std::string, RoomData> GameMenuState::loadLevelRoomData(
    const std::vector<std::string> &levelRooms)
{
    std::unordered_map<std::string, RoomData> map;

    for (const auto &rid : levelRooms)
    {
        RoomData rd = RoomLoader::loadFromId(rid, pauseTileMap.tileSize);
        map.emplace(rid, std::move(rd));
    }

    return map;
}

std::unordered_map<std::string, sf::Vector2i> GameMenuState::computeRoomOffsets(
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

void GameMenuState::buildPlacedRoomsFromOffsets(
    const std::unordered_map<std::string, RoomData> &roomDataMap,
    const std::unordered_map<std::string, sf::Vector2i> &offsets)
{
    pausePlacedRooms.clear();

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

        pausePlacedRooms.push_back({rd, off});
    }

    // compute bounds
    int minX = INT_MAX, minY = INT_MAX,
        maxX = INT_MIN, maxY = INT_MIN;

    for (auto &pr : pausePlacedRooms)
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
        pauseLevelSizeTiles = {0, 0};
        return;
    }

    for (auto &pr : pausePlacedRooms)
    {
        pr.offsetTiles -= sf::Vector2i(minX, minY);
    }

    pauseLevelSizeTiles = {maxX - minX, maxY - minY};
}

void GameMenuState::buildPauseTileMap()
{
    if (pauseLevelSizeTiles.x <= 0 ||
        pauseLevelSizeTiles.y <= 0)
    {
        pauseMapReady = false;
        return;
    }

    pauseTileMap.initEmpty(pauseLevelSizeTiles.x, pauseLevelSizeTiles.y);

    for (const auto &pr : pausePlacedRooms)
    {
        const RoomData &r = pr.room;
        sf::Vector2i dims = r.getRoomGridDimensions();
        sf::Vector2i off = pr.offsetTiles;

        for (int y = 0; y < dims.y; ++y)
        {
            for (int x = 0; x < dims.x; ++x)
            {
                pauseTileMap.setSymbolAt(x + off.x, y + off.y, r.tileGrid[y][x]);
            }
        }

        // Apply destroyed tiles
        auto destroyed = gameData.getDestroyedRoomTiles(r.roomId);
        for (const auto &tk : destroyed)
        {
            pauseTileMap.makeTileVoid(tk.x + off.x, tk.y + off.y);
        }
    }

    pauseTileMap.rebuildVertices();
    pauseMapReady = true;
}

void GameMenuState::preparePauseMap()
{
    const auto &world = gameData.getWorldData();
    const auto &currentRoom = gameData.getRoomData();

    auto it = world.roomToLevel.find(currentRoom.roomId);
    if (it == world.roomToLevel.end())
    {
        pauseMapReady = false;
        return;
    }

    const std::string &levelId = it->second;
    const auto &rooms = world.levels.at(levelId).rooms;
    if (rooms.empty())
    {
        pauseMapReady = false;
        return;
    }

    // start from the current room (or rooms.front())
    assemblePlacedRooms(rooms, currentRoom.roomId);
    buildPauseTileMap();
}
