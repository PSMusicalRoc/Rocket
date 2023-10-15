#pragma once

#include <RocketEngine.hpp>

/**
 * @class Paddle
 * 
 * This class really only stores the keys used for
 * the player to move the paddle, as well as the
 * paddle's vertical move speed.
*/
ROCKET_COMPONENT(Paddle,
    ROCKET_PROPERTY_DEFVAL(public, RocketKey, key_up, RocketKey::W)
    ROCKET_PROPERTY_DEFVAL(public, RocketKey, key_down, RocketKey::S)
    ROCKET_PROPERTY_DEFVAL(public, double, vertical_speed, 1.0)
);