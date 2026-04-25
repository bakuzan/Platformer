#include "MiniMap.h"

MiniMap::MiniMap(float width, float height)
    : width(width), height(height),
      borderThickness(3.f)
{
}

MiniMap::~MiniMap()
{
    // Destructor
}

// Publics

void MiniMap::updateView(float roomWidth, float roomHeight, const sf::RenderWindow &window)
{
    float leftPx = window.getSize().x - width - 10.f;
    float topPx = 10.f;

    border.setPosition(leftPx, topPx);
    border.setSize({width, height});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color(118, 118, 117));
    border.setOutlineThickness(borderThickness);

    float inset = border.getOutlineThickness();

    vpLeft = (leftPx + inset) / window.getSize().x;
    vpTop = (topPx + inset) / window.getSize().y;
    vpWidth = (width - inset * 2.f) / window.getSize().x;
    vpHeight = (height - inset * 2.f) / window.getSize().y;

    view.setCenter(roomWidth / 2.f, roomHeight / 2.f);
    view.setSize(roomWidth, roomHeight);
    view.setViewport(sf::FloatRect(vpLeft, vpTop, vpWidth, vpHeight));
}

void MiniMap::renderBorder(sf::RenderWindow &window)
{
    window.setView(window.getDefaultView());
    window.draw(border);
}
