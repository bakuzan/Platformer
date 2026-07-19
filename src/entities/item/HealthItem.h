#ifndef HEALTHITEM_H
#define HEALTHITEM_H

#include "constants/PickupSize.h"

#include "entities/Item.h"
#include "entities/Player.h"

class HealthItem : public Item
{
private:
    float healAmount;

public:
    HealthItem(float x, float y,
               PickupSize size);
    ~HealthItem();

    void onPickup(Player &player) override;
};

#endif // HEALTHITEM_H
