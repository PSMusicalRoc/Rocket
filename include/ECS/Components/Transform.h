#ifndef _ROC_TRANSFORM_COMPONENT_H_
#define _ROC_TRANSFORM_COMPONENT_H_

#include "ECS/Component.h"

struct Transform : public Component
{
    double x = 0, y = 0, z = 0;
};

#endif