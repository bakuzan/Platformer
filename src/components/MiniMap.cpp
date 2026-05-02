#include "MiniMap.h"

MiniMap::MiniMap(float width, float height)
    : width(width), height(height),
      borderThickness(3.f)
{
    background.setSize({width, height});
    background.setFillColor(sf::Color(118, 118, 117));
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
