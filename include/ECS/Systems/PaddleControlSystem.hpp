#pragma once

#include <RocketEngine.hpp>

class PaddleControls : public System
{
public:
    void Do(GLFWwindow* win)
    {
        Coordinator* cd = Coordinator::Get();
        for (Entity e : mEntities)
        {
            Transform& t = cd->GetComponent<Transform>(e);
            Paddle& p = cd->GetComponent<Paddle>(e);
            if (RocketKeyboard::IsKeyPressed(p.key_up))
            {
                t.y += p.vertical_speed;
            }
            if (RocketKeyboard::IsKeyPressed(p.key_down))
            {
                t.y -= p.vertical_speed;
            }
        }
    }

    Signature GetSignature() override
    {
        Signature sig;
        Coordinator* cd = Coordinator::Get();
        sig[cd->GetComponentType<Transform>()].flip();
        sig[cd->GetComponentType<Paddle>()].flip();
        return sig;
    }
};