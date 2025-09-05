#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <SFML/Graphics.hpp>

namespace GameUtils
{
    std::string padNumberAsText(int value, int width, char padChar);
    std::string formatScoreText(int score);

    sf::Vector2f getBottomRightPosition(const sf::RenderWindow &window,
                                        sf::Vector2f elementSize,
                                        float margin = 10.0f);

    sf::Vector2f normaliseVector(const sf::Vector2f &vector);

    float calculateEuclideanDistance(const sf::Vector2f &a,
                                     const sf::Vector2f &b);

    sf::Vector2f gridToWorld(int gridX, int gridY, float tileSize);

};

#endif // GAMEUTILS_H
