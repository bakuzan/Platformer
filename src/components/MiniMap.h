#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include <vector>

class MiniMap
{
public:
    MiniMap(float width, float height);
    ~MiniMap();

    sf::View &getView() { return view; }

    void updateView(float roomWidth, float roomHeight, const sf::RenderWindow &window);

    template <typename T>
    void renderWorld(sf::RenderWindow &window, T &world)
    {
        world.render(window);
    }

    template <typename T>
    void renderEntity(sf::RenderWindow &window, T &entity)
    {
        entity.render(window);
    }

    void renderBackground(sf::RenderWindow &window);
    void renderFog(sf::RenderWindow &window,
                   const std::vector<std::vector<bool>> &revealed,
                   float tileSize);

private:
    sf::View view;

    float width;
    float height;

    sf::Color blackoutColour;
    sf::RectangleShape background;
    float borderThickness;
    float vpLeft, vpTop, vpWidth, vpHeight;
};

#endif
