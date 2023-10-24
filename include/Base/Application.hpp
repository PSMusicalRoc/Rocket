/**
 * @file Application.hpp
 * 
 * This file defines the base Application class for the
 * Rocket Engine. Any application that wants to use
 * Rocket efficiently must subclass the Application class.
 * Inside this subclass, the user should:
 * 
 * <ul>
 *  <li>Initialize all Components and Systems</li>
 *  <li>Override the Main() function to define the loop</li>
 *  <li>Ensure that no memory leaks from the application by calling Application::FreeApplication()
 * on itself at the end of operation</li>
 * </ul>
 * 
 * @author Tim Bishop
*/

#ifndef _ROC_APPLICATION_GLFW_HPP_
#define _ROC_APPLICATION_GLFW_HPP_

/**
 * Defines the version of the Rocket Engine
 * internally
*/
#define ROCKET_VERSION "dev-entrypoint"

#ifndef __gl_h_
    #include <glad/gl.h>
#endif
#include <GLFW/glfw3.h>
#include <string>
#include "Roc_ECS.h"

#define InitComponent(comp) LogInfo("Registering " #comp "...");\
        if (!Coordinator::Get()->RegisterComponent<comp>()) { \
            LogError("Could not register " #comp "!"); \
        }


#define InitSystem(sys) LogInfo("Registering " #sys "...");\
        if (!Coordinator::Get()->RegisterSystem<sys>()) { \
            LogError("Could not register " #sys "!");\
        }\
        else {\
            Coordinator::Get()->SetSystemSignature<sys>(sys().GetSignature());\
        }

/**
 * @class Application
 * 
 * This class is the base class to any application the user
 * will want or need to create with Rocket. It provides a hook
 * for the user to instantiate a window, input handling, and
 * a main loop.
 * 
 * @note To learn more about this class, see @ref application_layout "this page here".
*/
class Application
{
protected:
    /**
     * The current application, mainly used for getting the width and height
     * of the current window. Less needed now that there is a unified coordinate
     * system.
     */
    static Application* m_currApp;

    /**
     * A flag checking if GLAD (OpenGL) has been loaded yet.
    */
    static bool loadedGLAD;

    /** The window's width, in pixels */
    int m_winWidth;
    /** The window's height, in pixels */
    int m_winHeight;
    /** The window's title */
    std::string m_winTitle;
    
    /**
     * The main GLFW window behind the application. Used internally all over
     * for input handling and rendering.
    */
    GLFWwindow* m_window;

    /**
     * Application constructor
     * 
     * @param appName The string name of the application's window
     * @param width The width of the new window, in pixels
     * @param height The height of the new window, in pixels
    */
    Application(const std::string& appName, int width, int height);

    /**
     * Virtual destructor - ensure that glfwDestroyWindow(m_window);
     * is called in any overrides of this function.
    */
    virtual ~Application() { glfwDestroyWindow(m_window); }

public:
    /**
     * This function should be used to create a new Application
     * in the main program. It should be overridden in the
     * subclass as well.
     * 
     * @param appName The string name of the application's window
     * @param width The width of the new window, in pixels
     * @param height The height of the new window, in pixels
     * 
     * @return A pointer to the newly created Application
    */
    Application* CreateApplication(const std::string& appName, int width, int height);
    /**
     * Destroys the application - Call this when the application is over
     * to avoid memory leaks! (Only if your application was created with
     * some form of `new Application()`!)
    */
    void FreeApplication() { delete this; }

    /**
     * Getter for the Application's width (in pixels)
     * 
     * @return The width of the window (in pixels)
    */
    int Width() const { return m_winWidth; }
    /**
     * Getter for the Application's height (in pixels)
     * 
     * @return The height of the window (in pixels)
    */
    int Height() const { return m_winHeight; }

    /**
     * Setter for the width of the window
     * 
     * @param w The new width (in pixels)
    */
    void SetWidth(int w) { m_winWidth = w; }
    /**
     * Setter for the height of the window
     * 
     * @param h The new height (in pixels)
    */
    void SetHeight(int h) { m_winHeight = h; }

    /**
     * Gets and returns the current Application.
     * 
     * @return A pointer to the current application, or nullptr
     * if it doesn't exist yet.
    */
    static Application* CurrentApplication() { return m_currApp; }

    /**
     * Virtual main function, override in subclasses.
    */
    virtual void Main() = 0;
};

namespace Rocket {
    extern Application* CreateApplication(const std::string&, int, int);
}

#endif