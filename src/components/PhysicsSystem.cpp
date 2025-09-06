#include <algorithm>

#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(const TileMap &map)
    : tileMap(map)
{
    // Constructor
}

PhysicsSystem::~PhysicsSystem()
{
    // Destructor
}

// Publics

PhysicsResult PhysicsSystem::moveAndCollide(
    const sf::FloatRect &bounds,
    sf::Vector2f velocity,
    float dt) const
{
    PhysicsResult result;
    result.velocity = velocity;
    result.grounded = false;
    sf::FloatRect newBounds = bounds;
    int tileSize = static_cast<int>(tileMap.tileSize);

    // Horizontal
    newBounds.left += velocity.x * dt;
    if (velocity.x != 0.f)
    {
        int startY = static_cast<int>(newBounds.top) / tileSize;
        int endY = static_cast<int>(newBounds.top + newBounds.height - 1) / tileSize;
        int tileX = (velocity.x > 0.0f)
                        ? static_cast<int>(newBounds.left + newBounds.width - 1.0f) / tileSize
                        : static_cast<int>(newBounds.left) / tileSize;

        for (int y = startY; y <= endY; ++y)
        {
            if (tileMap.isSolidTile(tileX, y))
            {
                if (velocity.x > 0)
                {
                    newBounds.left = tileX * tileMap.tileSize - newBounds.width;
                }
                else
                {
                    newBounds.left = (tileX + 1) * tileMap.tileSize;
                }

                result.velocity.x = 0.0f;
                break;
            }
        }
    }

    // Vertical
    newBounds.top += velocity.y * dt;
    if (velocity.y != 0.f)
    {
        int startX = static_cast<int>(newBounds.left) / tileSize;
        int endX = static_cast<int>(newBounds.left + newBounds.width - 1) / tileSize;
        constexpr float eps = 0.0001f;
        int tileY = (velocity.y > 0.0f)
                        ? static_cast<int>(newBounds.top + newBounds.height - eps) / tileSize
                        : static_cast<int>(newBounds.top) / tileSize;

        for (int x = startX; x <= endX; ++x)
        {
            if (tileMap.isSolidTile(x, tileY))
            {
                if (velocity.y > 0)
                {
                    newBounds.top = tileY * tileMap.tileSize - newBounds.height;
                    result.grounded = true;
                }
                else
                {
                    newBounds.top = (tileY + 1) * tileMap.tileSize;
                }

                result.velocity.y = 0.0f;
                break;
            }
        }
    }

    result.position = {newBounds.left, newBounds.top};
    return result;
}
