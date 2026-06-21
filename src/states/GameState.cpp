#include <format>
#include <iostream>

#include "utils/GameUtils.h"
#include "utils/InputUtils.h"
#include "utils/CollisionUtils.h"
#include "components/SettingsManager.h"
#include "components/RoomLoader.h"
#include "components/WorldLoader.h"
#include "constants/AudioId.h"
#include "constants/Constants.h"
#include "constants/PlayerState.h"
#include "data/GameOverStateConfig.h"

#include "GameState.h"
#include "GameMenuState.h"
#include "GameOverState.h"
#include "SaveMenuState.h"

GameState::GameState(GameData &data, StateManager &manager, sf::RenderWindow &win,
                     SaveData saveData)
    : gameData(data),
      stateManager(manager),
      window(win),
      camera(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT),
      tileMap(tileRegistry.createTileRegistry()),
      projectileRegistry(projectileRegistryCreator.createRegistry()),
      physicsSystem(tileMap),
      status(GameStatus::LOADING),
      uiManager(&window, data),
      canSaveHere(false)
{
    // Setup Input Manager
    inputManager.bind(Action::PAUSE, sf::Keyboard::Escape);
    inputManager.bind(Action::SAVE, sf::Keyboard::Enter);

    inputManager.bind(Action::MOVE_LEFT, sf::Keyboard::A);
    inputManager.bind(Action::MOVE_RIGHT, sf::Keyboard::D);
    inputManager.bind(Action::MOVE_UP, sf::Keyboard::W);
    inputManager.bind(Action::MOVE_DOWN, sf::Keyboard::S);
    inputManager.bind(Action::JUMP, sf::Keyboard::Space);
    inputManager.bind(Action::DROP_DOWN, sf::Keyboard::S);
    inputManager.bind(Action::DASH, sf::Keyboard::LShift);
    inputManager.bind(Action::SMASH, sf::Keyboard::S, true, Constants::SMASH_BUFFER_TIME);

    // Load world
    gameData.setWorldData(WorldLoader::load("resources/maps/world.txt"));

    // Setup player object
    auto player = std::make_shared<Player>(tileMap.tileSize * 0.95f);
    gameData.setPlayer(player);

    loadSaveState(saveData, player);
    loadMap(saveData.room, saveData.spawn, std::nullopt);
}

GameState::~GameState()
{
    // Destructor
}

// Publics

void GameState::handleEvent(const sf::Event &event)
{
    handleSystemEvents(event);
    handlePlayerEvents(event);

    uiManager.handleEvent(event);
}

void GameState::handleWindowResize(sf::Vector2u newSize)
{
    camera.handleResize(newSize.x, newSize.y);
    uiManager.handleResize(newSize.x, newSize.y);
}

void GameState::update(sf::Time deltaTime)
{
    // TODO restore when implemented
    // if (gameData.audioManager.getSoundStatus(AudioId::AMBIENT) != sf::Sound::Status::Playing)
    // {
    //     gameData.audioManager.playSound(AudioId::AMBIENT, true);
    // }

    float dt = deltaTime.asSeconds();

    auto &roomData = gameData.getRoomData();
    auto player = gameData.getPlayer();
    sf::FloatRect prevPlayerBounds = player->getBounds();

    // Update player
    bool leftHeld = inputManager.isDown(Action::MOVE_LEFT);
    bool rightHeld = inputManager.isDown(Action::MOVE_RIGHT);
    bool upHeld = inputManager.isDown(Action::MOVE_UP);
    bool downHeld = inputManager.isDown(Action::MOVE_DOWN);

    player->handleHorizontalInput(dt, leftHeld, rightHeld);
    player->handleVerticalInput(dt, upHeld, downHeld);

    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos, camera.getView());
    handlePlayerShooting(dt, mouseWorldPos);

    player->update(dt);

    PhysicsResult res = physicsSystem.moveAndCollide(
        player->getBounds(),
        player->getVelocity(),
        dt,
        player->getCapabilities());

    if (player->getPlayerState() == PlayerState::SMASHING &&
        res.tileProps.isBreakable)
    {
        processTileDestruction(roomData.roomId, res.tilePoint);
    }

    player->applyPhysicsResult(res);

    if (res.tileProps.isDamaging &&
        player->isTangible())
    {
        player->takeDamage(res.tileProps.damageAmount);
    }

    // Check collisions
    auto playerBounds = player->getBounds();
    auto playerPos = player->getPosition();
    checkSavePoints(roomData, playerBounds);
    if (checkEntrances(roomData, prevPlayerBounds, playerBounds))
    {
        return;
    }

    updateEnemies(dt, player);
    updateProjectiles(dt);
    updateItems(dt, player);
    revealTileOnMaps(playerPos, roomData);

    if (canSaveHere)
    {
        uiManager.showTooltip("Press " + inputManager.getKeyName(Action::SAVE) + " to Save",
                              {player->getPosition().x + (player->getBounds().width / 2.0f),
                               player->getPosition().y});
    }
    else
    {
        uiManager.clearTooltip();
    }

    // UI handling
    uiManager.update(tileMap, *player);

    // Re-centre
    sf::Vector2f roomDims = roomData.getRoomDimensions();
    camera.follow(
        player->getPosition(),
        roomDims.x, roomDims.y);

    // Finally, check if player died this loop
    if (player->isDead())
    {
        onPlayerDeath();
    }
}

void GameState::render()
{
    if (status == GameStatus::LOADING)
    {
        return;
    }

    // ---- MAIN GAME

    // Set view and render background
    camera.apply(window);
    tileMap.render(window);

    // Render entities
    auto &roomData = gameData.getRoomData();
    for (const auto &point : roomData.savePoints)
    {
        sf::FloatRect rect = GameUtils::getRectForRoomEntity(point, tileMap.tileSize);
        sf::RectangleShape safePoint(sf::Vector2f(rect.width, rect.height));
        safePoint.setFillColor(Constants::savepointColour);
        safePoint.setPosition(rect.left, rect.top);

        window.draw(safePoint);
    }

    auto player = gameData.getPlayer();
    player->render(window);

    auto &enemies = gameData.getEnemies();
    for (const auto &enemy : enemies)
    {
        enemy->render(window);
    }

    auto &items = gameData.getItems();
    for (const auto &item : items)
    {
        item->render(window);
    }

    // ---- UI Elements
    uiManager.render(tileMap);
}

// Private

void GameState::loadSaveState(SaveData &saveData, std::shared_ptr<Player> &player)
{
    for (const auto &ability : saveData.playerAbilities)
    {
        player->setAbility(ability);
    }

    for (const auto &room : saveData.destroyedTiles)
    {
        for (const auto &key : room.second)
        {
            gameData.markDestroyedTile(room.first, key.x, key.y);
        }
    }

    for (const auto &room : saveData.revealedTiles)
    {
        gameData.setRevealedTiles(room.first, room.second);
    }
}

void GameState::loadMap(const std::string mapRoomId,
                        const std::string playerSpawnKey,
                        std::optional<sf::Vector2i> passingOffset)
{
    status = GameStatus::LOADING;

    gameData.resetLevel();

    WorldData worldData = gameData.getWorldData();

    // Set roomData and load into map
    gameData.setRoomData(RoomLoader::loadFromId(mapRoomId, tileMap.tileSize));

    auto roomData = gameData.getRoomData();
    auto destroyedTiles = gameData.getDestroyedRoomTiles(mapRoomId);
    tileMap.loadFromRoom(roomData, destroyedTiles);

    std::cout << "Level: "
              << worldData.roomToLevel[roomData.roomId]
              << ", Room: "
              << roomData.roomId
              << std::endl;

    // Set player
    sf::Vector2f spawnPos = roomData.getPlayerSpawn(playerSpawnKey, passingOffset);
    gameData.getPlayer()->setSpawnPosition(spawnPos);

    // Process room entities
    auto player = gameData.getPlayer();
    auto &items = gameData.getItems();
    auto &enemies = gameData.getEnemies();

    roomData.processRoomEntities(player, items, enemies);

    applyEntranceClearance(roomData, playerSpawnKey);

    status = GameStatus::PLAYING;
}

void GameState::onPlayerDeath()
{
    stateManager.pushState(std::make_unique<GameOverState>(gameData, stateManager, window));
}

bool GameState::hasExited(const sf::FloatRect &playerBounds,
                          const sf::FloatRect &entranceRect,
                          const std::string &exitDir)
{
    if (!playerBounds.intersects(entranceRect))
    {
        return false;
    }

    float playerCenterX = playerBounds.left + (playerBounds.width / 2.f);
    float playerCenterY = playerBounds.top + (playerBounds.height / 2.f);

    if (exitDir == "up")
    {
        return playerCenterY < entranceRect.top;
    }
    else if (exitDir == "down")
    {
        return playerCenterY > (entranceRect.top + entranceRect.height);
    }
    else if (exitDir == "left")
    {
        return playerCenterX < entranceRect.left;
    }
    else if (exitDir == "right")
    {
        return playerCenterX > (entranceRect.left + entranceRect.width);
    }

    return false;
}

bool GameState::checkEntrances(const RoomData &currentRoom,
                               const sf::FloatRect &prevBounds,
                               const sf::FloatRect &newBounds)
{
    for (const auto &e : currentRoom.entrances)
    {
        sf::FloatRect rect = GameUtils::getRectForRoomEntity(e, tileMap.tileSize);
        const std::string &exitDir = e.properties.at("exitDir");

        sf::FloatRect sweep(
            std::min(prevBounds.left, newBounds.left),
            std::min(prevBounds.top, newBounds.top),
            std::max(prevBounds.left + prevBounds.width, newBounds.left + newBounds.width) -
                std::min(prevBounds.left, newBounds.left),
            std::max(prevBounds.top + prevBounds.height, newBounds.top + newBounds.height) -
                std::min(prevBounds.top, newBounds.top));

        if (hasExited(sweep, rect, exitDir))
        {
            const std::string &exitTarget = e.properties.at("target");
            std::optional<sf::Vector2i> passingOffset =
                computePassingOffset(e, newBounds, tileMap.tileSize);

            if (exitTarget == "END")
            {
                stateManager.pushState(
                    std::make_unique<GameOverState>(
                        gameData, stateManager, window,
                        GameOverStateConfig::init("Game Cleared!", sf::Color::Green)));
            }
            else
            {
                loadMap(exitTarget,
                        e.properties.at("spawn"),
                        passingOffset);
            }

            return true;
        }
    }

    return false;
}

std::optional<sf::Vector2i> GameState::computePassingOffset(
    const RoomEntity &e,
    const sf::FloatRect &playerBounds,
    float tileSize) const
{
    // If entrance has explicit spawn, no offset applies
    if (e.properties.contains("spawnX") ||
        e.properties.contains("spawnY"))
    {
        return std::nullopt;
    }

    const std::string &dir = e.properties.at("exitDir");
    sf::Vector2i offset(0, 0);
    sf::FloatRect rect = GameUtils::getRectForRoomEntity(e, tileSize);

    float cx = playerBounds.left + (playerBounds.width * 0.5f);
    float cy = playerBounds.top + (playerBounds.height * 0.5f);

    if (dir == "left" || dir == "right")
    {
        offset.y = static_cast<int>((cy - rect.top) / tileSize);
    }
    else if (dir == "up" || dir == "down")
    {
        offset.x = static_cast<int>((cx - rect.left) / tileSize);
    }
    else
    {
        return std::nullopt;
    }

    return offset;
}

void GameState::checkSavePoints(const RoomData &currentRoom,
                                const sf::FloatRect &playerBounds)
{
    canSaveHere = false;

    for (const auto &sp : currentRoom.savePoints)
    {
        sf::FloatRect rect = GameUtils::getRectForRoomEntity(sp, tileMap.tileSize);

        if (rect.intersects(playerBounds))
        {
            canSaveHere = true;
            auto it = sp.properties.find("spawn");
            if (it != sp.properties.end())
            {
                currentSavePoint = it->second;
            }
            break;
        }
    }
}

void GameState::handlePlayerEvents(const sf::Event &event)
{
    auto player = gameData.getPlayer();
    auto playerBounds = player->getBounds();

    if (event.type == sf::Event::KeyPressed &&
        inputManager.isPressed(Action::JUMP, event.key.code))
    {
        bool drop = inputManager.isDown(Action::DROP_DOWN);
        player->onJumpPressed(drop);
    }
    else if (event.type == sf::Event::KeyReleased &&
             inputManager.isPressed(Action::JUMP, event.key.code))
    {
        player->onJumpReleased();
    }

    if (event.type == sf::Event::KeyPressed &&
        player->hasAbility(PlayerAbility::DASH) &&
        inputManager.isPressed(Action::DASH, event.key.code))
    {
        player->onDashPressed();
    }

    if (event.type == sf::Event::KeyReleased &&
        player->hasAbility(PlayerAbility::SMASH) &&
        inputManager.isPressed(Action::SMASH, event.key.code))
    {
        player->onSmashPressed();
    }
}

void GameState::handlePlayerShooting(float dt, const sf::Vector2f &mouseWorldPos)
{
    (void)dt;

    auto projectiles = gameData.getProjectiles();
    auto player = gameData.getPlayer();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player->canShoot())
    {
        // Current weapon configurations
        ProjectileType currentAmmo = player->getCurrentAmmoType();
        const ProjectileConfig &config = projectileRegistry[currentAmmo];

        //  Calculate direction
        sf::Vector2f playerCenter = player->getCenter();
        sf::Vector2f targetDir = mouseWorldPos - playerCenter;
        sf::Vector2f normalizedDir = GameUtils::normaliseVector(targetDir);
        sf::Vector2f velocity = normalizedDir * config.speed;

        auto bullet = std::make_shared<Projectile>(
            config.size,
            playerCenter,
            velocity,
            config.damage,
            config.lifetime,
            Faction::PLAYER);

        projectiles.push_back(bullet);
        player->resetFireCooldown(config.fireRate);
    }
}

void GameState::handleSystemEvents(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed &&
        inputManager.isPressed(Action::PAUSE, event.key.code))
    {
        stateManager.pushState(std::make_unique<GameMenuState>(gameData, stateManager, window));
    }

    if (event.type == sf::Event::KeyPressed &&
        inputManager.isPressed(Action::SAVE, event.key.code))
    {
        if (canSaveHere)
        {
            stateManager.pushState(
                std::make_unique<SaveMenuState>(gameData, stateManager, window, currentSavePoint));
        }
    }
}

void GameState::applyEntranceClearance(const RoomData &currentRoom,
                                       const std::string &playerSpawnKey)
{
    // Find the entrance that matches the spawn key
    const RoomEntity *entrance = nullptr;
    for (const auto &e : currentRoom.entrances)
    {
        auto it = e.properties.find("spawn");
        if (it != e.properties.end() &&
            it->second == playerSpawnKey)
        {
            entrance = &e;
            break;
        }
    }

    if (entrance == nullptr)
    {
        return;
    }

    std::vector<sf::Vector2i> toClear;
    sf::IntRect r(
        entrance->x,
        entrance->y,
        std::stoi(entrance->properties.at("width")),
        std::stoi(entrance->properties.at("height")));

    auto dirIt = entrance->properties.find("exitDir");
    auto spawnYIt = entrance->properties.find("spawnY");

    if (dirIt != entrance->properties.end() &&
        spawnYIt != entrance->properties.end() &&
        dirIt->second == "up")
    {
        int spawnY = std::stoi(spawnYIt->second);

        // spawnY is below entrance, clear upwards across entrance width
        for (int y = r.top; y <= spawnY; ++y)
        {
            for (int x = r.left; x < r.left + r.width; ++x)
            {
                toClear.emplace_back(x, y);
            }
        }
    }

    for (auto &p : toClear)
    {
        auto props = tileMap.getTilePropertiesAtTile(p.x, p.y);

        if (props.has_value() &&
            props.value().isBreakable)
        {
            processTileDestruction(currentRoom.roomId, p);
        }
    }
}

void GameState::processTileDestruction(const std::string mapRoomId, sf::Vector2i &p)
{
    auto destroy = [&](int x)
    {
        tileMap.makeTileVoid(x, p.y);
        gameData.markDestroyedTile(mapRoomId, x, p.y);
    };

    // Destroy origin
    destroy(p.x);

    // Scan Left
    for (int x = p.x - 1; x >= 0; --x)
    {
        std::optional<TileProperties> props = tileMap.getTilePropertiesAtTile(x, p.y);

        if (props.has_value() &&
            props.value().isBreakable)
        {
            destroy(x);
        }
        else
        {
            break;
        }
    }

    // Scan Right
    int width = tileMap.getWidth();
    for (int x = p.x + 1; x < width; ++x)
    {
        std::optional<TileProperties> props = tileMap.getTilePropertiesAtTile(x, p.y);

        if (props.has_value() &&
            props.value().isBreakable)
        {
            destroy(x);
        }
        else
        {
            break;
        }
    }
}

void GameState::updateEnemies(float dt,
                              std::shared_ptr<Player> &player)
{
    auto &enemies = gameData.getEnemies();
    auto playerBounds = player->getBounds();
    auto playerPos = player->getPosition();
    bool playerTangible = player->isTangible();

    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
    {
        auto &enemy = **enemyIt;

        enemy.update(dt, playerPos);

        PhysicsResult enemyPhysics = physicsSystem.moveAndCollide(
            enemy.getBounds(),
            enemy.getVelocity(),
            dt,
            enemy.getCapabilities());

        enemy.applyPhysicsResult(enemyPhysics);

        if (enemy.isDead())
        {
            enemyIt = enemies.erase(enemyIt);
            continue;
        }

        auto enemyBounds = enemy.getBounds();

        if (playerTangible &&
            playerBounds.intersects(enemyBounds))
        {
            player->takeDamage(enemy.dealDamage());
        }

        ++enemyIt;
    }
}

void GameState::updateProjectiles(float dt)
{
    auto &projectiles = gameData.getProjectiles();
    auto &enemies = gameData.getEnemies();
    auto player = gameData.getPlayer();

    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        auto &proj = *it;

        proj->update(dt);

        if (proj->shouldRemove())
        {
            it = projectiles.erase(it);
            continue;
        }

        bool projectileDestroyed = false;
        auto projBounds = proj->getBounds();

        auto hitTile = physicsSystem.checkProjectileCollision(projBounds, proj->getVelocity(), dt);

        if (hitTile.has_value())
        {
            projectileDestroyed = true;
        }
        else
        {
            // Environmental Checks (e.g., Water)
            int centerTx = static_cast<int>((projBounds.left + projBounds.width * 0.5f) / tileMap.tileSize);
            int centerTy = static_cast<int>((projBounds.top + projBounds.height * 0.5f) / tileMap.tileSize);

            auto centerProps = tileMap.getTilePropertiesAtTile(centerTx, centerTy);

            if (centerProps.has_value() && centerProps.value().type == TileCategory::WATER)
            {
                proj->reduceLifetime(dt * 3.0f);
                proj->setVelocity(proj->getVelocity() * 0.95f);
            }
        }

        // Check Entity Collisions
        if (!projectileDestroyed)
        {
            if (proj->getFaction() == Faction::PLAYER)
            {
                for (auto &enemy : enemies)
                {
                    if (projBounds.intersects(enemy->getBounds()))
                    {
                        enemy->takeDamage(proj->getDamage());
                        projectileDestroyed = true;
                        break; // Stop checking other enemies, bullet is absorbed
                    }
                }
            }
            else if (proj->getFaction() == Faction::ENEMY)
            {
                if (player->isTangible() && projBounds.intersects(player->getBounds()))
                {
                    player->takeDamage(proj->getDamage());
                    projectileDestroyed = true;
                }
            }
        }

        // Cleanup or Continue
        if (projectileDestroyed)
        {
            it = projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void GameState::updateItems(float dt,
                            std::shared_ptr<Player> &player)
{
    (void)dt;

    auto &items = gameData.getItems();
    auto playerBounds = player->getBounds();

    for (auto itemIt = items.begin(); itemIt != items.end();)
    {
        auto &item = **itemIt;

        if (item.getBounds().intersects(playerBounds))
        {
            item.onPickup(*player);
            itemIt = items.erase(itemIt);
            continue;
        }
        else
        {
            ++itemIt;
        }
    }
}

void GameState::revealTileOnMaps(sf::Vector2f &playerPos, const RoomData &roomData)
{
    int tx = static_cast<int>(playerPos.x / tileMap.tileSize);
    int ty = static_cast<int>(playerPos.y / tileMap.tileSize);
    int radius = 3;

    for (int dy = -radius; dy <= radius; dy++)
    {
        for (int dx = -radius; dx <= radius; dx++)
        {
            gameData.revealTile(roomData.roomId, tx + dx, ty + dy);
        }
    }
}