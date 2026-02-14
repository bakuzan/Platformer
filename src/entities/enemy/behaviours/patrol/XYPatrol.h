#ifndef XYPATROL_H
#define XYPATROL_H

#include "PatrolBehaviour.h"

class Enemy;

class XYPatrol : public PatrolBehaviour
{
private:
    float leftX;
    float rightX;
    bool movingRight = true;

public:
    XYPatrol(float leftX, float rightX)
        : leftX(leftX), rightX(rightX) {}

    void patrol(Enemy &e, float dt) override
    {
        (void)dt;

        float x = e.getPosition().x;
        float vy = e.getVelocity().y;

        if (movingRight)
        {
            e.setVelocity({1.f, vy});

            if (x >= rightX)
            {
                movingRight = false;
            }
        }
        else
        {
            e.setVelocity({-1.f, vy});

            if (x <= leftX)
            {
                movingRight = true;
            }
        }
    }

    float getDistFromPatrol(const sf::Vector2f &enemyPos) const override
    {
        float patrolCenterX = (leftX + rightX) * 0.5f;
        return std::abs(enemyPos.x - patrolCenterX);
    }
};

#endif // XYPATROL_H