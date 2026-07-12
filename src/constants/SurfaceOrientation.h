#pragma once

enum class SurfaceOrientation
{
    ATTACHED_BOTTOM, // Floor is below enemy. Moving Right.
    ATTACHED_RIGHT,  // Wall is to the right. Moving Up.
    ATTACHED_TOP,    // Ceiling is above. Moving Left.
    ATTACHED_LEFT    // Wall is to the left. Moving Down.
};