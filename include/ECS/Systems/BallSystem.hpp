#pragma once

/**
 * @file BallSystem.hpp
 * 
 * This header file defines the class BallSystem,
 * the system the engine uses to move the ball, check
 * what the ball has collided with, and update velocities,
 * direction, and respawning.
 * 
 * @author Tim Bishop
*/

#include "Roc_ECS.h"
#include "ECS/Components/Ball.hpp"
#include "Base/Keyboard.hpp"
#include <cmath>

/**
 * @class BallSystem
 * 
 * This class is the system the engine uses to
 * move and update the ball.
*/
class BallSystem : public System
{
private:
/**
 * A helper function to check for where the center of
 * a rectangle collider is. It takes the offset
 * of the collider, as well as the transform of the
 * target and uses those to set two references with
 * the median x and y position of the rectangle collider.
 * 
 * @param rc The RectangleCollider of the entity
 * @param t The Transform of the entity
 * @param midX A reference to a double value that will be
 * set with the value of the midpoint of the Collider.
 * @param midY A reference to a double value that will be
 * set with the value of the midpoint of the Collider.
*/
    void CalculateCenterOfCollider(RectangleCollider& rc, Transform& t, double& midX, double& midY)
    {
        midX = (t.x + rc.offsetX + t.x + rc.offsetX + rc.width) / 2.0;
        midY = (t.y + rc.offsetY + t.y + rc.offsetY + rc.height) / 2.0;
    }

public:
/**
 * This is the workhorse of the system. It should be called every
 * frame and does the following:
 * <ul>
 *  <li>Checks if a given ball has just respawned. If so, sets the
 * new velocity accordingly.</li>
 *  <li>Checks if a given ball has collided with anything. If it has,
 * we update the ball's velocity accordingly.</li>
 *  <li>Check if the ball has hit a border of the screen. Left and right
 * borders indicate a score, top and bottom borders should simply invert
 * the Y velocity of the ball.</li>
 *  <li>Move the ball based on the current velocity and deltatime.</li>
 * </ul>
 * 
 * @brief Updates the ball's position and velocity based on collisions
 * 
 * @param deltatime A double representation of the time it took for the
 * previous frame to complete.
*/
    void Do(double deltatime)
    {
        Coordinator* cd = Coordinator::Get();
        for (Entity e : mEntities)
        {
            Transform& t = cd->GetComponent<Transform>(e);
            BallComponent& b = cd->GetComponent<BallComponent>(e);
            RectangleCollider& rc = cd->GetComponent<RectangleCollider>(e);

            if (b.justRespawned)
            {
                b.velocityX = b.velocity;
                b.justRespawned = false;
            }

            if (rc.collisions.size() != 0)
            {
                for (Collision& c : rc.collisions)
                {
                    double ballCX, ballCY;
                    double entCX, entCY;
                    Transform& ot = cd->GetComponent<Transform>(c.ent_collided);
                    RectangleCollider& orc = cd->GetComponent<RectangleCollider>(c.ent_collided);

                    CalculateCenterOfCollider(rc, t, ballCX, ballCY);
                    CalculateCenterOfCollider(orc, ot, entCX, entCY);

                    double angle = atan2(fabs(ballCY - entCY), fabs(ballCX - entCX)) * 0.5;

                    b.velocityX = b.velocityX < 0 ? cos(angle) * b.velocity : cos(angle) * -b.velocity;
                    b.velocityY = sin(angle) * b.velocity;
                }
            }

            //check for collision on the left or right border

            if (t.x + rc.offsetX + rc.width >= 160)
            {
                t.x = 160 - (rc.offsetX + rc.width);
                b.velocityX = -b.velocityX;
            }

            if (t.x + rc.offsetX <= 0)
            {
                t.x = rc.offsetX;
                b.velocityX = -b.velocityX;
            }

            //check for collision on the top or bottom border
            if (t.y + rc.offsetY + rc.height >= 90)
            {
                t.y = 90 - (rc.offsetY+ rc.height);
                b.velocityY = -b.velocityY;
            }

            if (t.y + rc.offsetY <= 0)
            {
                t.y = rc.offsetY;
                b.velocityY = -b.velocityY;
            }

            t.x += b.velocityX * deltatime;
            t.y += b.velocityY * deltatime;
        }
    }

    
    Signature GetSignature() override
    {
        Signature sig;
        Coordinator* cd = Coordinator::Get();
        sig[cd->GetComponentType<Transform>()].flip();
        sig[cd->GetComponentType<BallComponent>()].flip();
        sig[cd->GetComponentType<RectangleCollider>()].flip();
        return sig;
    }
};