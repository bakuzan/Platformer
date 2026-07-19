#include "constants/Constants.h"

#include "HealthItem.h"

HealthItem::HealthItem(float x, float y, PickupSize size)
    : Item(Constants::healthItemColour, x, y)
{
    switch (size)
    {
    case PickupSize::LARGE:
        healAmount = Constants::HEALTH_LARGE;
        break;
    case PickupSize::NORMAL:
        healAmount = Constants::HEALTH_NORMAL;
        break;
    case PickupSize::SMALL:
        healAmount = Constants::HEALTH_SMALL;
        break;
    }
}

HealthItem::~HealthItem()
{
    // Destructor
}

// Publics

void HealthItem::onPickup(Player &player)
{
    player.healDamage(healAmount);
}