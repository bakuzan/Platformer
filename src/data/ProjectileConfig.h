#pragma once

#include <SFML/Graphics.hpp>

struct ProjectileConfig
{
    float speed;
    float damage;
    float lifetime;
    float fireRate;
    sf::Vector2f size;
};