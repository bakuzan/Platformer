#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <SFML/Graphics.hpp>

#include "data/PhysicsResult.h"
#include "TileMap.h"

class PhysicsSystem
{
private:
    const TileMap &tileMap;

public:
    PhysicsSystem(const TileMap &map);
    ~PhysicsSystem();

    PhysicsResult moveAndCollide(
        const sf::FloatRect &bounds,
        sf::Vector2f velocity,
        float dt,
        bool ignoreTopPlatforms) const;
};

#endif // PHYSICSSYSTEM_H
