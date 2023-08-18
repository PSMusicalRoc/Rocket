#pragma once

/**
 * @file CoordinateMath.hpp
 * @author Tim Bishop
 *
 * @brief A header file with coordinate functions
 */

// Note that coordinates (0, 0) are in the top left corner
// of the screen!

float CoordConvert(float baseCoord, float screenSize)
{
    float retval = baseCoord / screenSize;
    return (retval - .5) * 2;
}

float GLCoordConvert(float glCoord, float screenSize)
{
    float retval = glCoord / 2 - .5;
    return retval * screenSize;
};
