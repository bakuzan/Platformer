#include <algorithm>
#include <optional>

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
    PlayerState playerState,
    const sf::FloatRect &bounds,
    sf::Vector2f velocity,
    float dt,
    bool ignoreTopPlatforms) const
{
    PhysicsResult result = PhysicsResult::create();
    result.velocity = velocity;

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
            const std::optional<TileProperties> &props = tileMap.getTilePropertiesAtTile(tileX, y);

            if (!props.has_value() ||
                props.value().solidity == Solidity::NONE ||
                props.value().solidity == Solidity::TOP) // One-way platforms are only solid vertically
            {
                continue;
            }

            if (props.value().solidity == Solidity::BOTH)
            {
                if (velocity.x > 0)
                {
                    newBounds.left = tileX * tileMap.tileSize - newBounds.width;
                    result.velocity.x = 0.0f;
                    result.touchingWallRight = true;
                }
                else
                {
                    newBounds.left = (tileX + 1) * tileMap.tileSize;
                    result.velocity.x = 0.0f;
                    result.touchingWallLeft = true;
                }

                break;
            }
        }
    }

    // Vertical
    newBounds.top += velocity.y * dt;

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
        const std::optional<TileProperties> &props = tileMap.getTilePropertiesAtTile(x, tileY);

        if (!props.has_value() ||
            props.value().solidity == Solidity::NONE ||
            (playerState == PlayerState::SMASHING && props.value().isBreakable))
        {
            continue;
        }

        if (props.value().solidity == Solidity::BOTH)
        {
            if (velocity.y > 0)
            {
                // Land
                newBounds.top = tileY * tileMap.tileSize - newBounds.height;
                result.grounded = true;
                result.velocity.y = 0.0f;
            }
            else if (velocity.y < 0)
            {
                // Ceiling bumped
                newBounds.top = (tileY + 1) * tileMap.tileSize;
                result.velocity.y = 0.0f;
            }
            break;
        }
        else if (props.value().solidity == Solidity::TOP)
        {
            if (ignoreTopPlatforms)
            {
                continue;
            }

            // Only collide if falling and crossing the top surface
            if (velocity.y > 0.f)
            {
                float tileTop = static_cast<float>(tileY * tileSize);

                bool crossing = (prevBottom <= tileTop) && (newBottom >= tileTop);
                bool fullyCleared = (newBottom > tileTop);

                if (crossing && fullyCleared)
                {
                    newBounds.top = tileTop - newBounds.height;
                    result.grounded = true;
                    result.velocity.y = 0.f;
                    break;
                }
            }
        }
    }

    // Final grounded check
    if (playerState != PlayerState::SMASHING)
    {
        int footY = static_cast<int>(newBounds.top + newBounds.height) / tileSize;
        int footXLeft = static_cast<int>(newBounds.left) / tileSize;
        int footXRight = static_cast<int>(newBounds.left + newBounds.width - 1) / tileSize;

        for (int x = footXLeft; x <= footXRight; ++x)
        {
            const auto &props = tileMap.getTilePropertiesAtTile(x, footY);
            if (!props.has_value())
            {
                continue;
            }

            if (props->solidity == Solidity::BOTH)
            {
                result.grounded = true;
                break;
            }

            if (props->solidity == Solidity::TOP &&
                !ignoreTopPlatforms)
            {
                float tileTop = static_cast<float>(footY * tileSize);

                bool crossing = (prevBottom <= tileTop) && (newBottom >= tileTop);
                bool fullyCleared = (newBottom > tileTop);

                if (crossing && fullyCleared)
                {
                    result.grounded = true;
                    break;
                }
            }
        }
    }

    // What are you in?
    sf::Vector2f samplePoint(newBounds.left + newBounds.width * 0.5f,
                             newBounds.top + newBounds.height * 0.5f);

    int sampleX = static_cast<int>(samplePoint.x) / tileSize;
    int sampleY = static_cast<int>(samplePoint.y) / tileSize;
    std::optional<TileProperties> sampleProps = tileMap.getTilePropertiesAtTile(sampleX, sampleY);

    result.tileProps = sampleProps.has_value() ? sampleProps.value() : TileProperties::makeEmpty();
    result.tilePoint = {sampleX, sampleY};
    result.position = {newBounds.left, newBounds.top};
    return result;
}
