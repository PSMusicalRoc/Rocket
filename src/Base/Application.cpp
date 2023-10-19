/**
 * @file Application.cpp
 * 
 * @brief Implementation for @link Application.hpp @endlink
*/

#include "Base/Application.hpp"
#include "ECS/Roc_ECS_Additions.hpp"
#include "RocLogger/RocLogger.hpp"

#include <iostream>

#include "Base/Keyboard.hpp"

bool Application::loadedGLAD = false;
Application* Application::m_currApp = nullptr;

Application::Application(const std::string& appName, int width, int height)
    :m_winTitle(appName), m_winWidth(width), m_winHeight(height)
{
    m_window = glfwCreateWindow(width, height, appName.c_str(), NULL, NULL);
    if (!m_window)
    {
        LogFatal("Could not create GLFW Window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    // Load OpenGL

    if (!loadedGLAD)
    {
        LogAssert(gladLoadGL(glfwGetProcAddress));
        LogInfo("GLAD Loaded!");
    }
    else
        LogInfo("GLAD already loaded!");

    // Set Keyboard Inputs

    glfwSetKeyCallback(m_window, RocketKeyboard::KeyCallback);   
}

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

    // we save gl loading for later, a window needs to
    // be created first

    LogTrace("Beginning Initialization of Components");
    InitComponent(Transform);
    InitComponent(Gravity);
    InitComponent(Sprite);
    InitComponent(RectangleCollider);
    LogTrace("Ending Initialization of Components");

    LogTrace("Begnning Initialization of Systems");
    InitSystem(RenderSpriteSystem);
    InitSystem(CollisionSystem);
    LogTrace("Ending Initialization of Systems");

    auto app = Rocket::CreateApplication("Test", 1920, 1080);
    app->Main();
    app->FreeApplication();

    LogTrace("All done!");
}
