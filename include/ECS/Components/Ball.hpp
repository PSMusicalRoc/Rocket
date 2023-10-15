#pragma once

#include "Roc_ECS.h"

/**
 * @class BallComponent
 * 
 * This class is the component attached to the ball
 * in the Pong clone I'm making. It contains data
 * values corresponding to its current velocity,
 * the respawn positions, as well as whether or not
 * the ball has just respawned.
*/
ROCKET_COMPONENT(BallComponent,
    ROCKET_PROPERTY(public, double, velocityX)
    ROCKET_PROPERTY(public, double, velocityY)
    ROCKET_PROPERTY_DEFVAL(public, double, velocity, 20.0)

    ROCKET_PROPERTY_DEFVAL(public, double, respawnX, 79.0)
    ROCKET_PROPERTY_DEFVAL(public, double, respawnY, 44.0)

    ROCKET_PROPERTY_DEFVAL(public, bool, justRespawned, true)
);