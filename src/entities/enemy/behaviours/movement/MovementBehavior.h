#ifndef MOVEMENTBEHAVIOUR_H
#define MOVEMENTBEHAVIOUR_H

class Enemy;

class MovementBehavior
{
public:
    virtual ~MovementBehavior() = default;

    virtual void move(Enemy &e, float dt, float speed) = 0;
};

#endif // MOVEMENTBEHAVIOUR_H