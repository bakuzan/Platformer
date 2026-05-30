#pragma once

#include "AttackProfile.h"

struct AttackProfile
{
    float triggerRange;      // where you start telegraphing
    float verticalTolerance; // |Δy| allowed
};
