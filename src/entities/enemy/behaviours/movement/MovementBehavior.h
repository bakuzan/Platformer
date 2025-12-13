#include "entities/Enemy.h"

class MovementBehavior
{
public:
    virtual ~MovementBehavior() = default;

    virtual void move(Enemy &e, float dt, float speed) = 0;
};
