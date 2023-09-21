#include "Roc_GL/CoordinateSystem.hpp"

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