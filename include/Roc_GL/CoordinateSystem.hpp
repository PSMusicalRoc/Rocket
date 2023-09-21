#pragma once

/**
 * # The Coordinate System
 * 
 * The coordinate system assumes, by default, a 16:9 aspect ratio. To that end, there are 160 units
 * of width and 90 units of length. If the width : height ratio is larger than 16:9, assume black
 * bars are on the left and right of the screen. If the ratio is less than 16:9, assume black bars are
 * on the top and bottom of the screen. In other words, screen space will always be 16:9.
 * 
 * Remember, there are multiple coordinate systems at play here:
 * 
 * | Coordinate System | Origin | Left to Right | Bottom to Top |
 * |-------------------|--------|---------------|---------------|
 * | OpenGL|Center|-1 to 1|-1 to 1|
 * | OpenGL TexCoord|Top Left|0 to 1|0 to 1|
 * | My System|Bottom Left|0 to 160|0 to 90|
*/
struct CoordinateSystem
{
    static int screen_width;
    static int screen_height;
};

double ConvertEngineXToGL(double x);
double EX2GL(double x);

double ConvertEngineYToGL(double y);
double EY2GL(double y);
