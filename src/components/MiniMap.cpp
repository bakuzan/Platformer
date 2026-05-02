#include "constants/Constants.h"
#include "MiniMap.h"

MiniMap::MiniMap(float width, float height)
    : width(width), height(height),
      borderThickness(3.f),
      blackoutColour(Constants::uiBackground)
{
    background.setSize({width, height});
    background.setFillColor(blackoutColour);
}

MiniMap::~MiniMap()
{
    // Destructor
}

// Publics

void MiniMap::updateView(float roomWidth, float roomHeight, const sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
    float leftPx = winSize.x - width - 10.f;
    float topPx = 10.f;
    float offset = borderThickness;

    background.setPosition(leftPx, topPx);

    vpLeft = (leftPx + offset) / winSize.x;
    vpTop = (topPx + offset) / winSize.y;
    vpWidth = (width - (2 * offset)) / winSize.x;
    vpHeight = (height - (2 * offset)) / winSize.y;

    view.setCenter(roomWidth / 2.f, roomHeight / 2.f);
    view.setSize(roomWidth, roomHeight);
    view.setViewport(sf::FloatRect(vpLeft, vpTop, vpWidth, vpHeight));
}

void MiniMap::renderBackground(sf::RenderWindow &window)
{
    window.draw(background);
}

void MiniMap::renderFog(sf::RenderWindow &window,
                        const std::vector<std::vector<bool>> &revealed,
                        float tileSize)
{
    sf::RectangleShape fog({tileSize, tileSize});
    fog.setFillColor(blackoutColour);

    for (int y = 0; y < revealed.size(); y++)
    {
        for (int x = 0; x < revealed[y].size(); x++)
        {
            if (!revealed[y][x])
            {
                fog.setPosition(x * tileSize, y * tileSize);
                window.draw(fog);
            }
        }
    }
}
