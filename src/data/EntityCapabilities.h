#pragma once

struct EntityCapabilities
{
    bool canBreakBreakables = false;
    bool ignoreTopPlatforms = false;

    static EntityCapabilities create(bool canBreak, bool ignoreSolidityTop)
    {
        return {canBreak, ignoreSolidityTop};
    }
};