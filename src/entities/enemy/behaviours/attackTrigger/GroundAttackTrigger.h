#ifndef GROUNDATTACKTRIGGER_H
#define GROUNDATTACKTRIGGER_H

#include "entities/enemy/behaviours/attackTrigger/AttackTriggerBehaviour.h"

#include "utils/GameUtils.h"

class GroundAttackTrigger : public AttackTriggerBehaviour
{
private:
    float triggerRange; // horizontal

public:
    GroundAttackTrigger(float range)
        : triggerRange(range) {}

    bool shouldAttack(const Enemy &e,
                      const sf::Vector2f &playerPos) override
    {
        sf::Vector2f enemyCenter = GameUtils::getCentre(e.getCollider());
        float dx = std::abs(playerPos.x - enemyCenter.x);

        return dx <= triggerRange &&
               e.canReach(playerPos);
    }
};

#endif // GROUNDATTACKTRIGGER_H