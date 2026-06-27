#include "ProjectileRegistry.h"

ProjectileRegistry::ProjectileRegistry()
{
    // Constructor
}

ProjectileRegistry::~ProjectileRegistry()
{
    // Destructor
}

// Publics

std::unordered_map<ProjectileType, ProjectileConfig> ProjectileRegistry::createRegistry()
{
    std::unordered_map<ProjectileType, ProjectileConfig> projectileRegistry = {
        {ProjectileType::STANDARD,
         ProjectileConfig::createProjectileConfig(800.f,
                                                  15.f,
                                                  0.45f,
                                                  0.25f,
                                                  {8.f, 8.f})},
    };

    return projectileRegistry;
}
