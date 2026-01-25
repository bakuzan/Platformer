#ifndef GROUNDATTACKTRIGGER_H
#define GROUNDATTACKTRIGGER_H

#include "entities/enemy/behaviours/attackTrigger/AttackTriggerBehaviour.h"

#include "utils/GameUtils.h"

class GroundAttackTrigger : public AttackTriggerBehaviour
{
private:
    float radius;

public:
    GroundAttackTrigger(float r)
        : radius(r) {}

    bool shouldAttack(const Enemy &e,
                      const sf::Vector2f &playerPos) override
    {
        sf::Vector2f pos = e.getPosition();
        float dist = GameUtils::getDistanceBetween(pos, playerPos);

        return dist < radius &&
               e.canReach(playerPos);
    }
};

#endif // GROUNDATTACKTRIGGER_H