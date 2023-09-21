#define STB_IMAGE_IMPLEMENTATION
#include "Logger/RocLogger.hpp"
#include "ECS/Roc_ECS.h"
#include <time.h>
#include <wait.h>
#include "Base/Application.hpp"
#include "ECS/Systems/LogTransform.hpp"
#include <cmath>

#include <Roc_GL/Texture.hpp>

#define InitSystem(sys) LogInfo("Registering " #sys "...");\
        if (!cd->RegisterSystem<sys>()) { \
            LogError("Could not register " #sys "!");\
        }\
        else {\
            cd->SetSystemSignature<sys>(sys().GetSignature());\
        }

class RocApplication : public Application
{
private:
    RocApplication(const std::string& appName, int width, int height)
        :Application(appName, width, height) {}
    
    unsigned int m_framerate = 60;

public:
    static RocApplication* CreateApplication(const std::string& appName, int width, int height)
    {
        RocApplication* app = new RocApplication(appName, width, height);
        if (m_currApp != nullptr)
            m_currApp->FreeApplication();
        
        Coordinator* cd = Coordinator::Get();
        LogTrace("Loading Systems...");
        InitSystem(LogTransform);
        LogTrace("End Loading Systems...");
        
        Application::m_currApp = app;
        return app;
    }

    void Main() override
    {
        Coordinator* cd = Coordinator::Get();
        Entity ent = cd->CreateEntity();
        cd->AddComponent<Transform>(ent, Transform());

        double curr_time = glfwGetTime();
        double prev_time = glfwGetTime();
        double deltatime = 0;

        while (!glfwWindowShouldClose(m_window))
        {
            curr_time = glfwGetTime();
            deltatime = curr_time - prev_time;
            //LogInfo("FrameTime: " + std::to_string(deltatime));
            LogInfo("FPS: " + std::to_string((int)(1/deltatime)));
            if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            {
                Transform& t = cd->GetComponent<Transform>(ent);
                t.x += 4;
            }
            if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            {
                Transform& t = cd->GetComponent<Transform>(ent);
                t.x -= 4;
            }
            if (glfwGetKey(m_window, GLFW_KEY_G) == GLFW_PRESS)
            {
                cd->GetSystem<LogTransform>()->Do();
            }

            glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            glfwPollEvents();
            glfwSwapBuffers(m_window);

            double timediff = glfwGetTime() - curr_time;
            double calculation = std::max((double)(1.0 / m_framerate) - timediff, 0.0);
            usleep((int)(calculation * 1000000.0));

            prev_time = curr_time;
        }
        FreeApplication();
    }
};


int main()
{
    LogTrace("Welcome to Rocket Engine v0.0.1a!");

    // Init GLFW

    LogTrace("Initializing GLFW");
    if (!glfwInit())
    {
        LogFatal("Could not initialize GLFW!");
        const char** ptr; glfwGetError(ptr);
        LogError(*ptr);
        return -1;
    }
    LogInfo("GLFW Initiated!");

    RocApplication* app = RocApplication::CreateApplication("Test", 1000, 1000);
    app->Main();
    //app->FreeApplication();

    LogTrace("All done!");
}