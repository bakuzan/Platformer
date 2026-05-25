#pragma once

struct JumpAttackProfile
{
    float idealHorizontalRange; // where you expect the jump to land
    float triggerRange;         // where you start telegraphing
    float verticalTolerance;    // |Δy| allowed
};
