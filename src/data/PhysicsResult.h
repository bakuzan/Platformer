#pragma once

#include <SFML/Graphics.hpp>

struct PhysicsResult
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool grounded;
};