#include "Roc_GL/CoordinateSystem.hpp"

/**
 * @file CoordinateSystem.cpp
 * 
 * @brief Implementation for @link CoordinateSystem.hpp @endlink
*/

int CoordinateSystem::screen_height = 0;
int CoordinateSystem::screen_width = 0;

double ConvertEngineXToGL(double x)
{
    return (x / (double)80) - 1.0;
}

double EX2GL(double x) { return ConvertEngineXToGL(x); }

double ConvertEngineYToGL(double y)
{
    return (y / (double)45) - 1.0;
}

double EY2GL(double y) { return ConvertEngineYToGL(y); }


/* PIXELS TO/FROM OPENGL */


double PixelsToOpenGLX(int pixels)
{
    double retval = (double)pixels / (double) CoordinateSystem::screen_width;
    return 2*retval - 1.0;
}

double P2GLX(int pixels) { return PixelsToOpenGLX(pixels); }

double PixelsToOpenGLY(int pixels)
{
    double retval = (double)pixels / (double) CoordinateSystem::screen_height;
    return 2*retval - 1.0;
}

double P2GLY(int pixels) { return PixelsToOpenGLY(pixels); }


/* ENGINE TO/FROM PIXELS */

double PixelsToEngineX(int pixels)
{
    double ratio = (double)CoordinateSystem::screen_width / 
        (double)CoordinateSystem::screen_height;
    
    if (ratio >= (double)16/(double)9)
    {
        // width is greater than the allowed 16:9 ratio
        // calculate how much space each side of the window
        // loses to a black bar.
        double black_bar_size = (double)CoordinateSystem::screen_width -
            (double)CoordinateSystem::screen_height * (double)16/(double)9;

        // Set new width and position and do calculations with
        // those values in mind
        int screen_width = CoordinateSystem::screen_width - (int)black_bar_size;
        int position = pixels - (int)(black_bar_size / 2);

        return (double)position / (double)screen_width * 160.0;
    }
    else
    {
        // height is greater than the allowed 16:9 ratio
        // therefore we can use the actual screen width

        return (double)pixels / (double)CoordinateSystem::screen_width * 160.0;
    }
}

double P2EX(int pixels) { return PixelsToEngineX(pixels); }

double PixelsToEngineY(int pixels)
{
    double ratio = (double)CoordinateSystem::screen_width / 
        (double)CoordinateSystem::screen_height;
    
    if (ratio >= (double)16/(double)9)
    {
        // width is greater than the allowed 16:9 ratio
        // therefore, we can use the window's height
        // directly
        return (double)pixels / (double)CoordinateSystem::screen_height * 90.0;
    }
    else
    {
        // height is greater than the allowed 16:9 ratio
        // calculate how much space each side of the window
        // loses to a black bar.
        double black_bar_size = (double)CoordinateSystem::screen_height -
            (double)CoordinateSystem::screen_width * (double)9/(double)16;

        // Set new width and position and do calculations with
        // those values in mind
        int screen_height = CoordinateSystem::screen_height - (int)black_bar_size;
        int position = pixels - (int)(black_bar_size / 2);

        return (double)position / (double)screen_height * 90.0;
    }
}

double P2EY(int pixels) { return PixelsToEngineY(pixels); }