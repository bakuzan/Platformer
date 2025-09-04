#ifndef TILEREGISTRY_H
#define TILEREGISTRY_H

#include <unordered_map>

#include "data/TileDefinition.h"

class TileRegistry
{
public:
    TileRegistry();
    ~TileRegistry();

    std::unordered_map<char, TileDefinition> createTileRegistry();
};

#endif // TILEREGISTRY_H
