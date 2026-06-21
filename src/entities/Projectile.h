#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

#include "constants/Faction.h"

class Projectile
{
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float damage;
    float lifetime;
    Faction faction;
    bool isDead;

public:
    Projectile(sf::Vector2f size, sf::Vector2f position, sf::Vector2f velocity,
               float damage, float lifetime,
               Faction faction);
    ~Projectile();

    void update(float dt);
    void render(sf::RenderWindow &window);
    void destroy();

    sf::FloatRect getBounds() const;
    float getDamage() const;
    Faction getFaction() const;
    bool shouldRemove() const;

    sf::Vector2f getVelocity() const;
    void setVelocity(const sf::Vector2f &newVelocity);
    void reduceLifetime(float amount);
};

#endif // PROJECTILE_H
