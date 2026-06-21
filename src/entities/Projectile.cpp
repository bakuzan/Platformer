#include "Projectile.h"

Projectile::Projectile(sf::Vector2f size, sf::Vector2f position, sf::Vector2f velocity,
                       float damage, float lifetime,
                       Faction faction)
    : velocity(velocity),
      damage(damage), lifetime(lifetime),
      faction(faction),
      isDead(false)
{
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(faction == Faction::PLAYER ? sf::Color::Yellow : sf::Color::Red);
}

Projectile::~Projectile()
{
    // Destructor
}

// Publics

void Projectile::update(float dt)
{
    lifetime -= dt;
    if (lifetime <= 0.f)
    {
        isDead = true;
    }

    shape.move(velocity * dt);
}

void Projectile::render(sf::RenderWindow &window)
{
    window.draw(shape);
}

void Projectile::destroy()
{
    isDead = true;
}

sf::FloatRect Projectile::getBounds() const
{
    return shape.getGlobalBounds();
}

float Projectile::getDamage() const
{
    return damage;
}

Faction Projectile::getFaction() const
{
    return faction;
}

bool Projectile::shouldRemove() const
{
    return isDead;
}

sf::Vector2f Projectile::getVelocity() const
{
    return velocity;
}

void Projectile::setVelocity(const sf::Vector2f &newVelocity)
{
    velocity = newVelocity;
}

void Projectile::reduceLifetime(float amount)
{
    lifetime -= amount;

    if (lifetime < 0.0f)
    {
        lifetime = 0.0f;
    }
}