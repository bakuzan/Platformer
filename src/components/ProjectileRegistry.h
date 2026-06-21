#ifndef PROJECTILEREGISTRY_H
#define PROJECTILEREGISTRY_H

#include <unordered_map>

#include "constants/ProjectileType.h"
#include "data/ProjectileConfig.h"

class ProjectileRegistry
{
public:
    ProjectileRegistry();
    ~ProjectileRegistry();

    std::unordered_map<ProjectileType, ProjectileConfig> createRegistry();
};

#endif // PROJECTILEREGISTRY_H
