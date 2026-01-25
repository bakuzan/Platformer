#ifndef FLYINGATTACKTRIGGER_H
#define FLYINGATTACKTRIGGER_H

#include "entities/enemy/behaviours/attackTrigger/AttackTriggerBehaviour.h"

class FlyingAttackTrigger : public AttackTriggerBehaviour
{
private:
    float horizontalRange;   // how close in X before diving
    float verticalTolerance; // how forgiving in Y

public:
    FlyingAttackTrigger(float hRange, float vTol)
        : horizontalRange(hRange),
          verticalTolerance(vTol) {}

    bool shouldAttack(const Enemy &e,
                      const sf::Vector2f &playerPos) override
    {
        sf::Vector2f pos = e.getPosition();

        float dx = std::abs(playerPos.x - pos.x);
        float dy = std::abs(playerPos.y - pos.y);

        bool horizontallyAligned = dx < horizontalRange;
        bool verticallyCloseEnough = dy < verticalTolerance;

        return horizontallyAligned &&
               verticallyCloseEnough &&
               e.canReach(playerPos);
    }
};

#endif // FLYINGATTACKTRIGGER_H