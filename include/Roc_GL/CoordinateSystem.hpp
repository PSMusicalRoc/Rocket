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
 * @todo Remove this class and its properties, they serve
 * zero function to the overall design of the engine.
*/
struct CoordinateSystem
{
    static int screen_width;
    static int screen_height;
};

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
double EX2GL(double x);


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
double EY2GL(double y);
