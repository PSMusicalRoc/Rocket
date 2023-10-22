#define STB_IMAGE_IMPLEMENTATION
#include <RocketEngine.hpp>
#include "ECS/Roc_ECS_Additions.hpp"

#include <cassert>
#include <string>
#include <iostream>

#include <time.h>
#include <wait.h>

class RocTests : public Application
{
private:
    unsigned int m_framerate = 60;

public:
    RocTests(const std::string& appName, int width, int height)
        :Application(appName, width, height)
    {
        if (m_currApp != nullptr)
            m_currApp->FreeApplication();

        LogInfo("Beginning initialization of user created components");
        LogInfo("Ending initialization of user created components");

        LogInfo("Beginning initialization of user created systems");
        LogInfo("Ending initialzation of user created systems");
        
        Application::m_currApp = this;
    }

    void Main() override
    {
        double speed = 0.5;

        double curr_time = glfwGetTime();
        double prev_time = glfwGetTime();
        double deltatime = 0.0;

        {
            for (int i = 0; i < 10; i++)
            {
                std::string new_key = "tex_" + std::to_string(i);
                LoadTexture("res/test.jpg", new_key);
            }

            for (int i = 0; i < 10; i++)
            {
                std::cout << "Image test: \"tex_" << i << "\"" << std::endl;
                std::cout << Textures::TextureMap.at(std::string("tex_") + std::to_string(i));
                std::cout << std::endl;
            }

            Textures::clear();
        }

        {
            for (int i = 0; i < 10; i++)
            {
                std::string new_key = "shad_" + std::to_string(i);
                LoadShader("res/sprite_vertshader.glsl", "res/sprite_fragshader.glsl", new_key);
            }

            Shaders::clear();
        }

        TextureInfo image_test;
        LoadTexture("res/logo.png", "test_sprite");
        image_test = Textures::TextureMap.at("test_sprite");

        LoadTexture("res/test.jpg", "test2");

        Shader sprite_shader;
        sprite_shader = LoadShader("res/sprite_vertshader.glsl", "res/sprite_fragshader.glsl", "sprite_shader");

        std::cout << "Image Test" << std::endl;
        std::cout << image_test << std::endl;

        Coordinator* cd = Coordinator::Get();
        Entity test = cd->CreateEntity("testent1");
        cd->AddComponent<Transform>(test, Transform());
        cd->AddComponent<Sprite>(test, Sprite());

        Entity other = cd->CreateEntity("testent2");
        cd->AddComponent<Transform>(other, Transform());
        cd->AddComponent<Sprite>(other, Sprite());

        {
            Sprite& s = cd->GetComponent<Sprite>(test);
            s.shader_key = "sprite_shader";
            s.texture_key = "test_sprite";

            Sprite& s2 = cd->GetComponent<Sprite>(other);
            s2.shader_key = "sprite_shader";
            s2.texture_key = "test2";
            s2.width = 10;
            s2.height = 10;

            Transform& t = cd->GetComponent<Transform>(other);
            t.x = 50;
            t.y = 50;
        }

        LoadScene("res/testscene.rscene");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        while (!glfwWindowShouldClose(m_window))
        {
            curr_time = glfwGetTime();
            deltatime = curr_time - prev_time;
            //LogInfo("FrameTime: " + std::to_string(deltatime));
            //LogInfo("FPS: " + std::to_string((int)(1/deltatime)));

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

namespace Rocket{
    Application* CreateApplication(const std::string& name, int width, int height)
    {
        return new RocTests(name, width, height);
    }
}