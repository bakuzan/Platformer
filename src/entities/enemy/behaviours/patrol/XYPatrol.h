#ifndef XYPATROL_H
#define XYPATROL_H

#include "PatrolBehaviour.h"

class Enemy;

class XYPatrol : public PatrolBehavior
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
        float x = e.shape->getPosition().x;

        if (movingRight)
        {
            e.velocity = {1.f, 0.f};

            if (x >= rightX)
            {
                movingRight = false;
            }
        }
        else
        {
            e.velocity = {-1.f, 0.f};

            if (x <= leftX)
            {
                movingRight = true;
            }
        }
    }
};

#endif // XYPATROL_H