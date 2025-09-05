#include <algorithm>

#include "Camera.h"

Camera::Camera(float viewWidth, float viewHeight)
    : baseWidth(viewWidth), baseHeight(viewHeight)
{
    view.setSize(baseWidth, baseHeight);
    view.setCenter(baseWidth / 2.f, baseHeight / 2.f);
}

Camera::~Camera()
{
    // Destructor
}

// Publics

void Camera::follow(const sf::Vector2f &targetPos, float roomWidth, float roomHeight)
{
    sf::Vector2f clamped = clampCenter(targetPos, roomWidth, roomHeight);
    view.setCenter(clamped);
}

void Camera::apply(sf::RenderWindow &window) const
{
    window.setView(view);
}

void Camera::handleResize(unsigned int windowWidth, unsigned int windowHeight)
{
    float windowAspect = static_cast<float>(windowWidth) / windowHeight;
    float viewAspect = baseWidth / baseHeight;

    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);
    const float epsilon = 0.001f;

    if (std::abs(windowAspect - viewAspect) > epsilon)
    {
        if (windowAspect > viewAspect)
        {
            // Window is wider than view
            float scale = viewAspect / windowAspect;
            viewport.left = (1.f - scale) / 2.f;
            viewport.width = scale;
        }
        else
        {
            // Window is taller than view
            float scale = windowAspect / viewAspect;
            viewport.top = (1.f - scale) / 2.f;
            viewport.height = scale;
        }
    }

    view.setViewport(viewport);
    view.setSize(baseWidth, baseHeight); // fixed zoom
}

// Privates

sf::Vector2f Camera::clampCenter(const sf::Vector2f &target, float roomWidth, float roomHeight) const
{
    float halfW = view.getSize().x / 2.f;
    float halfH = view.getSize().y / 2.f;

    float clampedX = std::max(halfW, std::min(target.x, roomWidth - halfW));
    float clampedY = std::max(halfH, std::min(target.y, roomHeight - halfH));

    return {clampedX, clampedY};
}
