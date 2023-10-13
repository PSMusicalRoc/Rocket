#pragma once

#include "Roc_ECS.h"
#include "ECS/Components/Ball.hpp"
#include "Base/Keyboard.hpp"
#include <cmath>

class BallSystem : public System
{
private:
    void CalculateCenterOfCollider(RectangleCollider& rc, Transform& t, double& midX, double& midY)
    {
        midX = (t.x + rc.offsetX + t.x + rc.offsetX + rc.width) / 2.0;
        midY = (t.y + rc.offsetY + t.y + rc.offsetY + rc.height) / 2.0;
    }

public:
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