#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <SFML/Graphics.hpp>

#include "constants/PlayerState.h"
#include "data/EntityCapabilities.h"
#include "data/PhysicsResult.h"
#include "TileMap.h"

class PhysicsSystem
{
private:
    const TileMap &tileMap;

private:
    void processHorizontalCollisions(sf::FloatRect &newBounds,
                                     sf::Vector2f &velocity,
                                     float dt,
                                     int tileSize,
                                     PhysicsResult &result) const;

    void processVerticalCollisions(const sf::FloatRect &oldBounds,
                                   sf::FloatRect &newBounds,
                                   sf::Vector2f velocity,
                                   float dt,
                                   int tileSize,
                                   PhysicsResult &result,
                                   EntityCapabilities capabilities) const;

    void checkGroundedState(const sf::FloatRect &oldBounds,
                            sf::FloatRect &newBounds,
                            int tileSize,
                            PhysicsResult &result,
                            EntityCapabilities capabilities) const;

    const PhysicsResult &constructPhysicsResult(sf::FloatRect &newBounds,
                                                int tileSize,
                                                PhysicsResult &result) const;

public:
    PhysicsSystem(const TileMap &map);
    ~PhysicsSystem();

    PhysicsResult moveAndCollide(
        PlayerState playerState,
        const sf::FloatRect &bounds,
        sf::Vector2f velocity,
        float dt,
        bool ignoreTopPlatforms) const;

    PhysicsResult moveAndCollide(
        const sf::FloatRect &bounds,
        sf::Vector2f velocity,
        float dt,
        bool ignoreTopPlatforms) const;
};

#endif // PHYSICSSYSTEM_H
