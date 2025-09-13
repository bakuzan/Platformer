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
        constexpr float eps = 0.0001f;

        float newLeft = newBounds.left;
        float newRight = newBounds.left + newBounds.width;

        int tileX = (velocity.x > 0.f)
                        ? static_cast<int>(newRight - eps) / tileSize
                        : static_cast<int>(newLeft) / tileSize;

        for (int y = startY; y <= endY; ++y)
        {
            const TileProperties &props = tileMap.getTilePropertiesAtTile(tileX, y);

            if (props.solidity == Solidity::NONE)
            {
                continue;
            }

            if (props.solidity == Solidity::BOTH)
            {
                if (velocity.x > 0)
                {
                    newBounds.left = tileX * tileMap.tileSize - newBounds.width;
                    result.velocity.x = 0.0f;
                }
                else
                {
                    newBounds.left = (tileX + 1) * tileMap.tileSize;
                    result.velocity.x = 0.0f;
                }

                break;
            }
            else if (props.solidity == Solidity::TOP)
            {
                // One-way platforms are only solid vertically
                continue;
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

        // For crossing detection
        float prevBottom = bounds.top + bounds.height;
        float newBottom = newBounds.top + newBounds.height;
        float newTop = newBounds.top;

        int tileY = (velocity.y > 0.f)
                        ? static_cast<int>(newBottom - eps) / tileSize
                        : static_cast<int>(newTop) / tileSize;

        for (int x = startX; x <= endX; ++x)
        {
            const TileProperties &props = tileMap.getTilePropertiesAtTile(x, tileY);

            if (props.solidity == Solidity::NONE)
            {
                continue;
            }

            if (props.solidity == Solidity::BOTH)
            {
                if (velocity.y > 0)
                {
                    newBounds.top = tileY * tileMap.tileSize - newBounds.height;
                    result.grounded = true;
                    result.velocity.y = 0.0f;
                }
                else
                {
                    newBounds.top = (tileY + 1) * tileMap.tileSize;
                    result.velocity.y = 0.0f;
                }
                break;
            }
            else if (props.solidity == Solidity::TOP)
            {
                // Only collide if falling and crossing the top surface
                if (velocity.y > 0.f)
                {
                    float tileTop = static_cast<float>(tileY * tileSize);
                    if (prevBottom <= tileTop &&
                        newBottom >= tileTop)
                    {
                        newBounds.top = tileTop - newBounds.height;
                        result.grounded = true;
                        result.velocity.y = 0.f;
                        break;
                    }
                }
            }
        }
    }

    result.position = {newBounds.left, newBounds.top};
    return result;
}
