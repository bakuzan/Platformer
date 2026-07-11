#ifndef RADIALATTACKTRIGGER_H
#define RADIALATTACKTRIGGER_H

#include "AttackTriggerBehaviour.h"
#include "utils/GameUtils.h"

class RadialAttackTrigger : public AttackTriggerBehaviour
{
private:
    float triggerRadius;

public:
    RadialAttackTrigger(float radius)
        : triggerRadius(radius)
    {
    }

    bool shouldAttack(const Enemy &e,
                      const sf::Vector2f &playerPos) override
    {
        sf::Vector2f enemyCenter = GameUtils::getCentre(e.getCollider());
        float distance = GameUtils::getDistanceBetween(enemyCenter, playerPos);

        return distance <= triggerRadius;
    }
};

#endif // RADIALATTACKTRIGGER_H