#pragma once

#include "Roc_ECS.h"
#include "ECS/Components/Ball.hpp"

class BallSystem : public System
{
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
                b.velocityX = 20.0;
                b.justRespawned = false;
            }

            if (rc.collisions.size() != 0)
            {
                for (Collision& c : rc.collisions)
                {
                    b.velocityX = -b.velocityX;
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