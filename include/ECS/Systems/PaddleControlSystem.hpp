#pragma once

#include <GLFW/glfw3.h>
#include <Roc_ECS.h>

class PaddleControls : public System
{
public:
    void Do(GLFWwindow* win)
    {
        Coordinator* cd = Coordinator::Get();
        for (Entity e : mEntities)
        {
            Transform& t = cd->GetComponent<Transform>(e);
            if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
            {
                t.x += 0.5;
            }
            if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
            {
                t.x -= 0.5;
            }
            if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
            {
                t.y += 0.5;
            }
            if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
            {
                t.y -= 0.5;
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