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
        sf::FloatRect b = e.getBounds();
        sf::Vector2f enemyCenter(b.left + b.width * 0.5f,
                                 b.top + b.height * 0.5f);

        float dx = std::abs(playerPos.x - enemyCenter.x);

        return dx <= triggerRange &&
               e.canReach(playerPos);
    }
};

#endif // GROUNDATTACKTRIGGER_H