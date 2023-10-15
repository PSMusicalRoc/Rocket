#pragma once

/**
 * @file PaddleControlSystem.hpp
 * 
 * This file implements a very simple System to
 * control the user's paddles.
 * 
 * @author Tim Bishop
*/

#include <RocketEngine.hpp>

/**
 * @class PaddleControls
 * 
 * This class controls the movement of the Paddles currently
 * in the scene. It relies on the RocketKeys set for movement
 * as well as the speed at which each paddle should be able
 * to move.
*/
class PaddleControls : public System
{
public:
    /**
     * @todo Add deltatime to this function, it really should be
     * tied to the framerate.
     * 
     * @todo Remove GLFWwindow* from arguments list, now that
     * the keyboard works correctly, we don't need it.
     * 
     * This function is the powerhouse of the System. It should be
     * called every frame. It loops over every Paddle in the scene,
     * and depending on if the key for the paddle moving up or down
     * is held, the paddle will move. Its speed is controlled by the
     * Paddle's Paddle::vertical_speed field.
     * 
     * @param win A pointer to the current GLFWwindow (deprecated)
    */
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