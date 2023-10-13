#define STB_IMAGE_IMPLEMENTATION
#include <RocketEngine.hpp>
#include <time.h>
#include <wait.h>

#include "ECS/Roc_ECS_Additions.hpp"

//#include "cereal/details/helpers.hpp"
#include <fstream>
#include <cmath>

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
        
        LogInfo("Beginning initialization of user created components");
        InitComponent(Paddle);
        InitComponent(BallComponent);
        LogInfo("Ending initialization of user created components");

        LogInfo("Beginning initialization of user created systems");
        InitSystem(PaddleControls);
        InitSystem(BallSystem);
        LogInfo("Ending initialzation of user created systems");
        
        Application::m_currApp = app;
        return app;
    }

    void Main() override
    {
        double curr_time = glfwGetTime();
        double prev_time = glfwGetTime();
        double deltatime = 0;

        Coordinator* cd = Coordinator::Get();

        LoadScene("res/pong.rscene");

        while (!glfwWindowShouldClose(m_window))
        {
            curr_time = glfwGetTime();
            deltatime = curr_time - prev_time;

            cd->GetSystem<CollisionSystem>()->Do();

            cd->GetSystem<PaddleControls>()->Do(m_window);

            cd->GetSystem<BallSystem>()->Do(deltatime);

            cd->GetSystem<CollisionSystem>()->Clear();

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            cd->GetSystem<RenderSpriteSystem>()->Do();

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

    RocApplication* app = RocApplication::CreateApplication("Test", 1920, 1080);
    app->Main();
    //app->FreeApplication();

    LogTrace("All done!");
}
