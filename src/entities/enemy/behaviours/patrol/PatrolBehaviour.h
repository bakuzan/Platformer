#ifndef PATROLBEHAVIOUR_H
#define PATROLBEHAVIOUR_H

class Enemy;

class PatrolBehaviour
{
public:
    virtual ~PatrolBehaviour() = default;

    // sets e.velocity based on patrol logic
    virtual void patrol(Enemy &e, float dt) = 0;

    // returns a value that will allow enemy to decide whether to "give up the chase"
    virtual float getDistFromPatrol(const sf::Vector2f &enemyPos) const = 0;
};

#endif // PATROLBEHAVIOUR_H