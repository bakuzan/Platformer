#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera
{
private:
    sf::View view;
    float baseWidth;
    float baseHeight;

private:
    sf::Vector2f clampCenter(const sf::Vector2f &target, float roomWidth, float roomHeight) const;

public:
    Camera(float viewWidth, float viewHeight);
    ~Camera();

    void follow(const sf::Vector2f &targetPos, float roomWidth, float roomHeight);
    void apply(sf::RenderWindow &window) const;

    void handleResize(unsigned int windowWidth, unsigned int windowHeight);

    sf::View &getView();
    const sf::View &getView() const;
};

#endif // CAMERA_H
