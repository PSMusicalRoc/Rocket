#pragma once

#include <GLFW/glfw3.h>
#include <Roc_ECS.h>
#include "Base/Keyboard.hpp"

ROCKET_COMPONENT(Paddle,
    ROCKET_PROPERTY_DEFVAL(public, RocketKey, key_up, RocketKey::W)
    ROCKET_PROPERTY_DEFVAL(public, RocketKey, key_down, RocketKey::S)
    ROCKET_PROPERTY_DEFVAL(public, double, vertical_speed, 1.0)
);