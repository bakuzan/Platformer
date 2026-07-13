#ifndef CRAWLERENEMY_H
#define CRAWLERENEMY_H

#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>

#include "constants/Constants.h"
#include "entities/Enemy.h"
#include "entities/enemy/behaviours/movement/SurfaceMovement.h"
#include "entities/enemy/behaviours/patrol/SurfacePatrol.h"

class CrawlerEnemy : public Enemy
{
private:
    int numBodySegments = 3;
    int historyFramesPerSegment = 7;

    std::deque<sf::Vector2f> positionHistory;
    std::vector<sf::RectangleShape> bodyVisuals;
    std::vector<sf::FloatRect> bodyColliders;

public:
    CrawlerEnemy(const sf::Vector2f &pos)
    {
        sf::Vector2f size(24.f, 24.f);
        setCollider(size, pos);

        shapeColour = sf::Color::Red;
        visualShape = new sf::RectangleShape(size);
        visualShape->setFillColor(shapeColour);
        visualShape->setPosition(pos);

        // Setup the Body Segments
        for (int i = 0; i < numBodySegments; ++i)
        {
            sf::RectangleShape segment(size);
            segment.setFillColor(sf::Color(static_cast<sf::Uint8>(240 - (i * 40)), 0, 0));
            segment.setPosition(pos);
            bodyVisuals.push_back(segment);
        }

        bodyColliders.resize(numBodySegments);

        // Init history
        int requiredHistory = numBodySegments * historyFramesPerSegment;
        for (int i = 0; i < requiredHistory; ++i)
        {
            positionHistory.push_back(pos);
        }

        medium = MovementMedium::SURFACE_CRAWLER;
        int startingHealth = 40;
        maxHealth = startingHealth;
        health = startingHealth;

        patrolSpeed = 200.f;

        // Peaceful crawler - no aggro!
        aggroRadius = 0.f;
        chaseRadius = 0.f;

        attackDamage = 20.f; // For touch

        patrol = new SurfacePatrol();
        movement = new SurfaceMovement();

        chase = nullptr;
        attack = nullptr;
        attackTrigger = nullptr;
    }

    std::vector<sf::FloatRect> getExtraColliders() const override
    {
        return bodyColliders;
    }

    void applyEnvironmentForces(float dt) override
    {
        (void)dt;

        if (auto *surfacePatrol = dynamic_cast<SurfacePatrol *>(patrol))
        {
            sf::Vector2f stickyGravDir = surfacePatrol->getGravityDir();

            // Apply a gentle "gravity" force into the wall so turning doesn't break
            float stickyForce = 80.f;

            sf::Vector2f currentVel = getVelocity();
            currentVel.x += stickyGravDir.x * stickyForce;
            currentVel.y += stickyGravDir.y * stickyForce;

            setVelocity(currentVel);
        }
    }

    void applyPhysicsResult(PhysicsResult &res) override
    {
        sf::Vector2f beforeVel = velocity;
        Enemy::applyPhysicsResult(res);

        if (auto *surfacePatrol = dynamic_cast<SurfacePatrol *>(patrol))
        {
            surfacePatrol->handlePhysics(beforeVel, res);
        }
    }

    void update(float dt, const sf::Vector2f &playerPos, const TileMap &map) override
    {
        Enemy::update(dt, playerPos, map);

        // Manage position history
        positionHistory.push_front(getPosition());

        int maxHistory = (numBodySegments + 1) * historyFramesPerSegment;
        if (positionHistory.size() > maxHistory)
        {
            positionHistory.pop_back();
        }

        // Update body segments
        bool isFlashing = (damageFlashTimer > 0.f);
        for (int i = 0; i < numBodySegments; ++i)
        {
            int historyIndex = (i + 1) * historyFramesPerSegment;

            if (historyIndex < positionHistory.size())
            {
                sf::Vector2f segPos = positionHistory[historyIndex];

                bodyVisuals[i].setPosition(segPos);
                bodyColliders[i] = sf::FloatRect(segPos.x, segPos.y,
                                                 collider.getSize().x, collider.getSize().y);

                if (isFlashing)
                {
                    bodyVisuals[i].setFillColor(Constants::damageTakenColour);
                }
                else
                {
                    int redCalc = std::max(0, 240 - (i * 40));
                    bodyVisuals[i].setFillColor(sf::Color(static_cast<sf::Uint8>(redCalc), 0, 0));
                }
            }
        }
    }

    void render(sf::RenderWindow &window)
    {
        Enemy::render(window);

        for (auto &seg : bodyVisuals)
        {
            window.draw(seg);
        }
    }
};

#endif // CRAWLERENEMY_H