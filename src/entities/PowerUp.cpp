#include "PowerUp.h"

PowerUp::PowerUp(float x, float y)
    : Item(sf::Color::Magenta, x, y)
{
}

PowerUp::~PowerUp()
{
    // Destructor
}

// Publics

PlayerAbility PowerUp::getType() const
{
    return type;
}

void PowerUp::onPickup(Player &player)
{
    player.setAbility(type);
}