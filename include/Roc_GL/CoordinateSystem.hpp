#pragma once

/**
 * @page coordsystem The Coordinate System
 * 
 * The coordinate system assumes, by default, a 16:9 aspect ratio. To that end, there are 160 units
 * of width and 90 units of length. If the width : height ratio is larger than 16:9, assume black
 * bars are on the left and right of the screen. If the ratio is less than 16:9, assume black bars are
 * on the top and bottom of the screen. In other words, screen space will always be 16:9.
 * 
 * Remember, there are multiple coordinate systems at play here:
 * 
 * | Coordinate %System | Origin | Left to Right | Bottom to Top |
 * |-------------------|--------|---------------|---------------|
 * | OpenGL|Center|-1 to 1|-1 to 1|
 * | OpenGL TexCoord|Top Left|0 to 1|0 to 1|
 * | Rocket Coordinate %System|Bottom Left|0 to 160|0 to 90|
 * 
 * Pretty much anywhere in the engine, if a property asks for a coordinate and doesn't explicitly state
 * what units the coordinate is in (ex. pixels, GL coordinate, etc.), the user can assume that the
 * coordinate is in terms of the Rocket Coordinate %System. Should your game need it, feel free to define
 * your own coordinate system that can go back and forth between it and the Rocket system.
 * 
 * @author Tim Bishop
*/

/**
 * @file CoordinateSystem.hpp
 * 
 * This file defines a basic Coordinate system in a
 * 16:9 aspect ratio. It also details a documentation
 * page defining the Coordinate Systems used by all
 * the different parts of the engine.
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
