#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "GameUtils.h"

namespace GameUtils
{
    std::string padNumberAsText(int value, int width, char padChar)
    {
        std::stringstream ss;
        ss << std::setw(width) << std::setfill(padChar) << value;
        return ss.str();
    }

    std::string formatScoreText(int score)
    {
        return padNumberAsText(score, 8, '0');
    }

    sf::Vector2f getBottomRightPosition(const sf::RenderWindow &window,
                                        sf::Vector2f elementSize,
                                        float margin)
    {
        sf::Vector2f viewSize = window.getDefaultView().getSize();

        return {viewSize.x - elementSize.x - margin,
                viewSize.y - elementSize.y - margin};
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

    float calculateEuclideanDistance(const sf::Vector2f &a,
                                     const sf::Vector2f &b)
    {
        sf::Vector2f d = b - a;
        return std::sqrt(d.x * d.x + d.y * d.y);
    }

    sf::Vector2f gridToWorld(int gridX, int gridY, float tileSize)
    {
        return sf::Vector2f(gridX * tileSize,
                            gridY * tileSize);
    }

}