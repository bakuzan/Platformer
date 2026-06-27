#pragma once

#include <SFML/Graphics.hpp>

struct ProjectileConfig
{
    float speed;
    float damage;
    float lifetime;
    float fireRate;
    sf::Vector2f size;

    static ProjectileConfig createProjectileConfig(float bulletSpeed,
                                                   float bulletDamage,
                                                   float bulletLifetime,
                                                   float bulletFireRate,
                                                   sf::Vector2f bulletSize)
    {
        return {bulletSpeed, bulletDamage,
                bulletLifetime, bulletFireRate,
                bulletSize};
    }
};