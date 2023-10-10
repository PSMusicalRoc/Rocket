#pragma once

#include "Roc_ECS.h"

ROCKET_COMPONENT(BallComponent,
    ROCKET_PROPERTY(public, double, velocityX)
    ROCKET_PROPERTY(public, double, velocityY)
    ROCKET_PROPERTY_DEFVAL(public, double, maxVelocityX, 10.0)
    ROCKET_PROPERTY_DEFVAL(public, double, maxVelocityY, 10.0)

    ROCKET_PROPERTY_DEFVAL(public, double, respawnX, 79.0)
    ROCKET_PROPERTY_DEFVAL(public, double, respawnY, 44.0)

    ROCKET_PROPERTY_DEFVAL(public, bool, justRespawned, true)
);