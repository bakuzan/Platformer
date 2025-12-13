#include "entities/Enemy.h"

class PatrolBehavior
{
public:
    virtual ~PatrolBehavior() = default;

    // sets e.velocity based on patrol logic
    virtual void patrol(Enemy &e, float dt) = 0;
};
