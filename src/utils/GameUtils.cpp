#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include "GameUtils.h"

namespace GameUtils
{
    std::string padNumberAsText(int value, int width, char padChar)
    {
        std::stringstream ss;
        ss << std::setw(width) << std::setfill(padChar) << value;
        return ss.str();
    }

    sf::Vector2f normaliseVector(const sf::Vector2f &vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        if (magnitude > 0)
        {
            return vector / magnitude;
        }

        return sf::Vector2f(0.0f, 0.0f);
    }

    float getDistanceBetween(const sf::Vector2f &a,
                             const sf::Vector2f &b)
    {
        sf::Vector2f d = a - b;
        return std::sqrt(d.x * d.x + d.y * d.y);
    }

    sf::FloatRect getRectForRoomEntity(const RoomEntity &e, float tileSize)
    {
        float w = (e.properties.count("width")
                       ? std::stoi(e.properties.at("width"))
                       : 1) *
                  tileSize;
        float h = (e.properties.count("height")
                       ? std::stoi(e.properties.at("height"))
                       : 1) *
                  tileSize;

        return sf::FloatRect(e.x * tileSize, e.y * tileSize, w, h);
    }

    char getTileSymbol(TileName name)
    {
        switch (name)
        {
        case TileName::BASIC_GROUND:
            return '#';
        case TileName::WEAK_GROUND:
            return '@';
        case TileName::WATER:
            return '~';
        case TileName::PLATFORM:
            return '-';
        case TileName::SCENERY:
            return '=';
        case TileName::PILLAR:
            return '_';
        case TileName::LAVA:
            return '+';
        case TileName::CLIMBABLE:
            return '|';
        case TileName::VOID:
        default:
            return '.';
        }
    }

}