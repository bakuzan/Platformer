#include "constants/Constants.h"

#include "AmmoItem.h"

AmmoItem::AmmoItem(float x, float y,
                   ProjectileType projectileType,
                   PickupSize size)
    : Item(Constants::ammoItemColour, x, y),
      ammoType(projectileType)

{
    switch (size)
    {
    case PickupSize::LARGE:
        ammoAmount = Constants::AMMO_HEAVY_SHOT_LARGE;
        break;
    case PickupSize::NORMAL:
        ammoAmount = Constants::AMMO_HEAVY_SHOT_NORMAL;
        break;
    case PickupSize::SMALL:
        ammoAmount = Constants::AMMO_HEAVY_SHOT_SMALL;
        break;
    }
}

AmmoItem::~AmmoItem()
{
    // Destructor
}

// Publics

void AmmoItem::onPickup(Player &player)
{
    player.addAmmo(ammoType, ammoAmount);
}