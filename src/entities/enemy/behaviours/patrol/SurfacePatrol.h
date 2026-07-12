#ifndef SURFACEPATROL_H
#define SURFACEPATROL_H

#include <SFML/Graphics.hpp>

#include "constants/SurfaceOrientation.h"
#include "entities/enemy/behaviours/patrol/PatrolBehaviour.h"

class SurfacePatrol : public PatrolBehaviour
{
private:
    SurfaceOrientation state = SurfaceOrientation::ATTACHED_BOTTOM;
    sf::Vector2f forwardDir = {1.f, 0.f};
    sf::Vector2f gravityDir = {0.f, 1.f};

    // Safety lock so they don't spin wildly when spawning mid-air
    bool wasAttached = false;

public:
    void patrol(Enemy &e, float dt) override
    {
        (void)dt;
        e.setVelocity(forwardDir);
    }

    void handlePhysics(const sf::Vector2f &beforeVel,
                       const PhysicsResult &res)
    {
        bool blockedX = (beforeVel.x != 0.f && res.velocity.x == 0.f);
        bool blockedY = (beforeVel.y != 0.f && res.velocity.y == 0.f);

        // Are we currently gripping our target wall?
        bool isStickyBlocked = false;
        if (gravityDir.x != 0.f && blockedX)
        {
            isStickyBlocked = true;
        }

        if (gravityDir.y != 0.f && blockedY)
        {
            isStickyBlocked = true;
        }

        if (isStickyBlocked)
        {
            wasAttached = true;
        }

        // Did we fall off a ledge? (Only valid if we were previously attached!)
        bool lostContact = false;
        if (wasAttached && !isStickyBlocked)
        {
            lostContact = true;
            wasAttached = false;
        }

        // Did we hit a wall in front of us?
        bool forwardBlocked = false;
        if (forwardDir.x != 0.f && blockedX)
        {
            forwardBlocked = true;
        }

        if (forwardDir.y != 0.f && blockedY)
        {
            forwardBlocked = true;
        }

        // State Machine
        if (forwardBlocked)
        {
            switch (state)
            {
            case SurfaceOrientation::ATTACHED_BOTTOM:
                setOrientation(SurfaceOrientation::ATTACHED_RIGHT);
                break;
            case SurfaceOrientation::ATTACHED_RIGHT:
                setOrientation(SurfaceOrientation::ATTACHED_TOP);
                break;
            case SurfaceOrientation::ATTACHED_TOP:
                setOrientation(SurfaceOrientation::ATTACHED_LEFT);
                break;
            case SurfaceOrientation::ATTACHED_LEFT:
                setOrientation(SurfaceOrientation::ATTACHED_BOTTOM);
                break;
            }
        }
        else if (lostContact)
        {
            switch (state)
            {
            case SurfaceOrientation::ATTACHED_BOTTOM:
                setOrientation(SurfaceOrientation::ATTACHED_LEFT);
                break;
            case SurfaceOrientation::ATTACHED_LEFT:
                setOrientation(SurfaceOrientation::ATTACHED_TOP);
                break;
            case SurfaceOrientation::ATTACHED_TOP:
                setOrientation(SurfaceOrientation::ATTACHED_RIGHT);
                break;
            case SurfaceOrientation::ATTACHED_RIGHT:
                setOrientation(SurfaceOrientation::ATTACHED_BOTTOM);
                break;
            }
        }
    }

    sf::Vector2f getGravityDir() const
    {
        return gravityDir;
    }

    float getDistFromPatrol(const sf::Vector2f &enemyPos) const override
    {
        (void)enemyPos;
        // NoOp
        return 0.f;
    }

private:
    void setOrientation(SurfaceOrientation newState)
    {
        state = newState;
        switch (state)
        {
        case SurfaceOrientation::ATTACHED_BOTTOM:
            forwardDir = {1.f, 0.f};
            gravityDir = {0.f, 1.f};
            break;
        case SurfaceOrientation::ATTACHED_RIGHT:
            forwardDir = {0.f, -1.f};
            gravityDir = {1.f, 0.f};
            break;
        case SurfaceOrientation::ATTACHED_TOP:
            forwardDir = {-1.f, 0.f};
            gravityDir = {0.f, -1.f};
            break;
        case SurfaceOrientation::ATTACHED_LEFT:
            forwardDir = {0.f, 1.f};
            gravityDir = {-1.f, 0.f};
            break;
        }
    }
};

#endif // SURFACEPATROL_H