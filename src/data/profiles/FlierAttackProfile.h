#pragma once

struct FlierAttackProfile
{
    float horizontalRange; // how close in X before diving
    float verticalRange;   // must be ABOVE player by at least this much
    float diveSpeed;       // base dive speed
    float maxDiveAngle;    // optional: limit stupid sideways dives
};
