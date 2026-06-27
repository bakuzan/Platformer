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
         ProjectileConfig::createProjectileConfig(600.f,
                                                  15.f,
                                                  2.0f,
                                                  0.25f,
                                                  {8.f, 8.f})},
    };

    return projectileRegistry;
}
