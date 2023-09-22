#ifndef _ROC_APPLICATION_GLFW_HPP_
#define _ROC_APPLICATION_GLFW_HPP_

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

#define InitComponent(comp) LogInfo("Registering " #comp "...");\
        if (!cd->RegisterComponent<comp>()) { \
            LogError("Could not register " #comp "!"); \
        }


#define InitSystem(sys) LogInfo("Registering " #sys "...");\
        if (!cd->RegisterSystem<sys>()) { \
            LogError("Could not register " #sys "!");\
        }\
        else {\
            cd->SetSystemSignature<sys>(sys().GetSignature());\
        }

class Application
{
protected:
    static Application* m_currApp;

    static bool loadedGLAD;

    int m_winWidth, m_winHeight;
    std::string m_winTitle;
    
    GLFWwindow* m_window;

    Application(const std::string& appName, int width, int height);
    virtual ~Application() { glfwDestroyWindow(m_window); }

public:
    Application* CreateApplication(const std::string& appName, int width, int height);
    void FreeApplication() { delete this; }

    int Width() const { return m_winWidth; }
    int Height() const { return m_winHeight; }

    void SetWidth(int w) { m_winWidth = w; }
    void SetHeight(int h) { m_winHeight = h; }

    static Application* CurrentApplication() { return m_currApp; }

    virtual void Main() = 0;
};

#endif