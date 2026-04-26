#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>

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

private:
    sf::View view;

    float width;
    float height;

    sf::RectangleShape background;
    float borderThickness;
    float vpLeft, vpTop, vpWidth, vpHeight;
};

#endif
