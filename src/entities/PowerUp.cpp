#include "PowerUp.h"

PowerUp::PowerUp(float x, float y, PlayerAbility playerAbility)
    : Item(sf::Color::Magenta, x, y),
      type(playerAbility)
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