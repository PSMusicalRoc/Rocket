#include "Base/Application.hpp"
#include "ECS/Roc_ECS.h"
#include "Logger/RocLogger.hpp"

#include <iostream>

bool Application::loadedGLAD = false;
Application* Application::m_currApp = nullptr;

Application::Application(const std::string& appName, int width, int height)
    :m_winTitle(appName), m_winWidth(width), m_winHeight(height)
{
    // Initialize components

    Coordinator* cd = Coordinator::Get();

    LogTrace("Beginning Initialization of Components");
    InitComponent(Transform);
    InitComponent(Gravity);
    InitComponent(Sprite);
    LogTrace("Ending Initialization of Components");

    LogTrace("Begnning Initialization of Systems");
    InitSystem(RenderSpriteSystem);
    LogTrace("Ending Initialization of Systems");

    m_window = glfwCreateWindow(width, height, appName.c_str(), NULL, NULL);
    if (!m_window)
    {
        LogFatal("Could not create GLFW Window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    if (!loadedGLAD)
    {
        LogAssert(gladLoadGL(glfwGetProcAddress))
        LogInfo("GLAD Loaded!");
    }
    else
        LogInfo("GLAD already loaded!");
}