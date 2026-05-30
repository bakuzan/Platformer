#ifndef FLYINGATTACKTRIGGER_H
#define FLYINGATTACKTRIGGER_H

#include "entities/enemy/behaviours/attackTrigger/AttackTriggerBehaviour.h"

class FlyingAttackTrigger : public AttackTriggerBehaviour
{
private:
    float horizontalRange; // how close in X before diving
    float verticalRange;   // how far ABOVE the player we must be

public:
    FlyingAttackTrigger(float hRange, float vRange)
        : horizontalRange(hRange),
          verticalRange(vRange)
    {
    }

    bool shouldAttack(const Enemy &e,
                      const sf::Vector2f &playerPos) override
    {
        sf::Vector2f pos = e.getPosition();

        float dx = std::abs(playerPos.x - pos.x);
        float dy = playerPos.y - pos.y; // If pos.y < playerPos.y, enemy is ABOVE the player.

        bool horizontallyAligned = false;
        if (dx <= horizontalRange)
        {
            horizontallyAligned = true;
        }

        bool aboveEnough = false;
        if (dy >= verticalRange)
        {
            aboveEnough = true;
        }

        if (horizontallyAligned &&
            aboveEnough &&
            e.canReach(playerPos))
        {
            return true;
        }

        return false;
    }
};

#endif // FLYINGATTACKTRIGGER_H
