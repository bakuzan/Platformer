#ifndef PATROLBEHAVIOUR_H
#define PATROLBEHAVIOUR_H

class Enemy;

class PatrolBehavior
{
public:
    virtual ~PatrolBehavior() = default;

    // sets e.velocity based on patrol logic
    virtual void patrol(Enemy &e, float dt) = 0;
};

#endif // PATROLBEHAVIOUR_H