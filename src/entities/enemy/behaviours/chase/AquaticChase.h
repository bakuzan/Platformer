#ifndef AQUATICCHASE_H
#define AQUATICCHASE_H

#include <SFML/Graphics.hpp>

#include "DefaultChase.h"
#include "entities/Enemy.h"

class AquaticChase : public DefaultChase
{
public:
    void chase(Enemy &e,
               float dt,
               sf::Vector2f playerPosition) override
    {
        // Drop aggro if the enemy is out of water
        if (!e.isSwimming())
        {
            e.setVelocity({0.f, 0.f});
            return;
        }

        DefaultChase::chase(e, dt, playerPosition);
    }
};

#endif // AQUATICCHASE_H