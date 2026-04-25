#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>

class MiniMap
{
public:
    MiniMap(float width, float height);
    ~MiniMap();

    void updateView(float roomWidth, float roomHeight, const sf::RenderWindow &window);

    template <typename T>
    void renderWorld(sf::RenderWindow &window, T &world)
    {
        sf::View prev = window.getView();
        window.setView(view);
        world.render(window);
        window.setView(prev);
    }

    template <typename T>
    void renderEntity(sf::RenderWindow &window, T &entity)
    {
        sf::View prev = window.getView();
        window.setView(view);
        entity.render(window);
        window.setView(prev);
    }

    void renderBorder(sf::RenderWindow &window);

private:
    sf::View view;

    float width;
    float height;

    sf::RectangleShape border;
    float borderThickness;
    float vpLeft, vpTop, vpWidth, vpHeight;
};

#endif
