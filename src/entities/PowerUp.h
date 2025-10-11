#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>

#include "constants/PlayerAbility.h"
#include "entities/Item.h"
#include "entities/Player.h"

class PowerUp : public Item
{
private:
    PlayerAbility type;

public:
    PowerUp(float x, float y);
    ~PowerUp();

    PlayerAbility getType() const;
    void onPickup(Player &player) override;
};

#endif // POWERUP_H
