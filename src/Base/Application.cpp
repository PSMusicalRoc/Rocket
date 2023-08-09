#include "Base/Application.hpp"
#include "Roc_ECS.h"
#include "RocLogger/RocLogger.hpp"

#include <iostream>

#define InitComponent(comp) LogInfo("Registering " #comp "...");\
        if (!cd->RegisterComponent<comp>()) { \
            LogError("Could not register " #comp "!"); \
        }

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
    LogTrace("Ending Initialization of Components");

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
        LogAssert(gladLoadGL(glfwGetProcAddress));
        LogInfo("GLAD Loaded!");
    }
    else
        LogInfo("GLAD already loaded!");
}
