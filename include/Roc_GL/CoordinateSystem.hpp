#pragma once

/**
 * @file CoordinateSystem.hpp
 * 
 * This file defines a basic Coordinate system in a
 * 16:9 aspect ratio. It also details a documentation
 * page defining the Coordinate Systems used by all
 * the different parts of the engine.
 * 
 * @note To learn more about the Coordinate System, see @ref coordsystem "this page here".
 * 
 * @author Tim Bishop
*/

/**
 * @class CoordinateSystem
 * 
 * Holds important data values (screen width and
 * height in pixels) so that the coordinate
 * system functions can successfully convert between
 * pixels, OpenGL, and Engine coordinates.
*/
struct CoordinateSystem
{
    static int screen_width;
    static int screen_height;
};


/* ENGINE TO/FROM OPENGL */

/**
 * This function converts the Rocket Coordinate
 * %System into OpenGL Coordinates.
 * 
 * @param x The coordinate to convert.
 * 
 * @return A coordinate in the OpenGL %System
 * representing the input coordinate.
*/
double ConvertEngineXToGL(double x);

/**
 * Stands for "Engine X to OpenGL". This is
 * a shortcut function to ConvertEngineXToGL().
 * 
 * @param x A coordinate in the Rocket Coordinate
 * %System.
 * 
 * @returns A coordinate in the OpenGL Coordinate
 * %System that is equal to the input.
*/
double E2GLX(double x);


/**
 * This function converts the Rocket Coordinate
 * %System into OpenGL Coordinates.
 * 
 * @param y The coordinate to convert.
 * 
 * @return A coordinate in the OpenGL %System
 * representing the input coordinate.
*/
double ConvertEngineYToGL(double y);

/**
 * Stands for "Engine Y to OpenGL". This is
 * a shortcut function to ConvertEngineYToGL().
 * 
 * @param y A coordinate in the Rocket Coordinate
 * %System.
 * 
 * @returns A coordinate in the OpenGL Coordinate
 * %System that is equal to the input.
*/
double E2GLY(double y);


/* PIXEL TO/FROM OPENGL */

/**
 * Converts a width in pixels to an x value in
 * OpenGL coordinates.
 * 
 * @param pixels The X value to convert
 * 
 * @returns The OpenGL coordinate (between -1 and 1)
*/
double PixelsToOpenGLX(int pixels);

/**
 * Converts a width in pixels to an x value in
 * OpenGL coordinates. (Shortcut for
 * PixelsToOpenGLX())
 * 
 * @param pixels The X value to convert
 * 
 * @returns The OpenGL coordinate (between -1 and 1)
*/
double P2GLX(int pixels);

/**
 * Converts a height in pixels to a y value in
 * OpenGL coordinates.
 * 
 * @param pixels The Y value to convert
 * 
 * @returns The OpenGL coordinate (between -1 and 1)
*/
double PixelsToOpenGLY(int pixels);

/**
 * Converts a height in pixels to a y value in
 * OpenGL coordinates. (Shortcut for
 * PixelsToOpenGLY())
 * 
 * @param pixels The Y value to convert
 * 
 * @returns The OpenGL coordinate (between -1 and 1)
*/
double P2GLY(int pixels);


/* ENGINE TO/FROM PIXELS */

double PixelsToEngineX(int pixels);

double P2EX(int pixels);

double PixelsToEngineY(int pixels);

double P2EY(int pixels);


int EngineToPixelsX(double coord);

int E2PX(double coord);

int EngineToPixelsY(double coord);

int E2PY(double coord);