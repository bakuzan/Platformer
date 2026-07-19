#ifndef AMMOITEM_H
#define AMMOITEM_H

#include "constants/ProjectileType.h"
#include "constants/PickupSize.h"

#include "entities/Item.h"
#include "entities/Player.h"

class AmmoItem : public Item
{
private:
    ProjectileType projectileType;
    float ammoAmount;

public:
    AmmoItem(float x, float y,
             ProjectileType projectileType,
             PickupSize size);
    ~AmmoItem();

    void onPickup(Player &player) override;
};

#endif // AMMOITEM_H
