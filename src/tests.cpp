#define STB_IMAGE_IMPLEMENTATION

#include "Roc_GL/Texture.hpp"
#include "Roc_GL/Shader.hpp"
#include "Roc_GL/CoordinateSystem.hpp"
#include "Logger/RocLogger.hpp"
#include "Base/Application.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <string>
#include <iostream>

#include <ECS/Systems/RenderSpriteSys.hpp>

#include <time.h>
#include <wait.h>

class RocTests : public Application
{
private:
    RocTests(const std::string& appName, int width, int height)
        :Application(appName, width, height) {}
    
    unsigned int m_framerate = 60;

public:
    static RocTests* CreateApplication(const std::string& appName, int width, int height)
    {
        RocTests* app = new RocTests(appName, width, height);
        if (m_currApp != nullptr)
            m_currApp->FreeApplication();
        
        Application::m_currApp = app;
        return app;
    }

    void Main() override
    {
        double speed = 0.5;

        double curr_time = glfwGetTime();
        double prev_time = glfwGetTime();
        double deltatime = 0;

        TextureInfo image_test;
        LoadTexture("res/logo.png", "test_sprite");
        image_test = Textures::TextureMap.at("test_sprite");

        LoadTexture("res/test.jpg", "test2");

        Shader sprite_shader;
        sprite_shader = LoadShader("res/sprite_vertshader.glsl", "res/sprite_fragshader.glsl", "sprite_shader");

        std::cout << "Image Test" << std::endl;
        std::cout << image_test << std::endl;

        Coordinator* cd = Coordinator::Get();
        Entity test = cd->CreateEntity();
        cd->AddComponent<Transform>(test, Transform());
        cd->AddComponent<Sprite>(test, Sprite());

        Entity other = cd->CreateEntity();
        cd->AddComponent<Transform>(other, Transform());
        cd->AddComponent<Sprite>(other, Sprite());

        {
            Sprite& s = cd->GetComponent<Sprite>(test);
            s._shader_key = "sprite_shader";
            s._tex_key = "test_sprite";

            Sprite& s2 = cd->GetComponent<Sprite>(other);
            s2._shader_key = "sprite_shader";
            s2._tex_key = "test2";
            s2.width = 10;
            s2.height = 10;

            Transform& t = cd->GetComponent<Transform>(other);
            t.x = 50;
            t.y = 50;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        while (!glfwWindowShouldClose(m_window))
        {
            curr_time = glfwGetTime();
            deltatime = curr_time - prev_time;
            //LogInfo("FrameTime: " + std::to_string(deltatime));
            //LogInfo("FPS: " + std::to_string((int)(1/deltatime)));

            if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            {
                cd->GetComponent<Transform>(other).x += speed;
            }
            if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            {
                cd->GetComponent<Transform>(other).x -= speed;
            }
            if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            {
                cd->GetComponent<Transform>(other).y += speed;
            }
            if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            {
                cd->GetComponent<Transform>(other).y -= speed;
            }

            glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            cd->GetSystem<RenderSpriteSystem>()->Do();

            glfwPollEvents();
            glfwSwapBuffers(m_window);

            double timediff = glfwGetTime() - curr_time;
            double calculation = std::max((double)(1.0 / m_framerate) - timediff, 0.0);
            usleep((int)(calculation * 1000000.0));

            prev_time = curr_time;
        }

        // Clear all loaded OpenGL things.
        Shaders::clear();
        Textures::clear();
    }

};

int main()
{
    LogTrace("Initializing GLFW");
    if (!glfwInit())
    {
        LogFatal("Could not initialize GLFW!");
        const char** ptr; glfwGetError(ptr);
        LogError(*ptr);
        return -1;
    }
    LogInfo("GLFW Initiated!");

    RocTests *app = RocTests::CreateApplication("window", 1280, 720);
    app->Main();

    app->FreeApplication();

    return 0;
}