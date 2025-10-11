#ifndef ROOMLOADER_H
#define ROOMLOADER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "data/RoomData.h"
#include "data/RoomEntity.h"

class RoomLoader
{
public:
    static RoomData loadFromFile(const std::string &filename,
                                 float tileSize)
    {
        RoomData room(tileSize);

        // Load map tiles
        loadTileGrid(filename, room);

        // Derive entities filename
        std::string entitiesFile = filename;
        auto dotPos = entitiesFile.find_last_of('.');

        if (dotPos != std::string::npos)
        {
            entitiesFile.insert(dotPos, "_entities");
        }
        else
        {
            entitiesFile += "_entities";
        }

        // Load entities
        loadEntities(entitiesFile, room);

        return room;
    }

    RoomEntity *findSpawn(RoomData &room, const std::string &spawnKey)
    {
        for (auto &entity : room.entities)
        {
            auto it = entity.properties.find("spawn");
            if (it != entity.properties.end() &&
                it->second == spawnKey)
            {
                return &entity;
            }
        }

        return nullptr;
    }

private:
    static void loadTileGrid(const std::string &filename,
                             RoomData &room)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("RoomLoader: Failed to open map file: " + filename);
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty())
            {
                room.tileGrid.push_back(line);
            }
        }
    }

    static void loadEntities(const std::string &filename,
                             RoomData &room)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("RoomLoader: Failed to open entities file: " + filename);
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }

            RoomEntity entity;
            auto eqPos = line.find('=');
            if (eqPos == std::string::npos)
            {
                continue;
            }

            entity.type = line.substr(0, eqPos);
            std::string rest = line.substr(eqPos + 1);

            auto semiPos = rest.find(';');
            std::string coords = (semiPos == std::string::npos) ? rest : rest.substr(0, semiPos);
            std::stringstream coordStream(coords);
            std::string xStr, yStr;
            std::getline(coordStream, xStr, ',');
            std::getline(coordStream, yStr, ',');

            entity.x = std::stoi(xStr);
            entity.y = std::stoi(yStr);

            if (semiPos != std::string::npos)
            {
                std::string props = rest.substr(semiPos + 1);
                std::stringstream propStream(props);
                std::string pair;
                while (std::getline(propStream, pair, ';'))
                {
                    auto kvPos = pair.find('=');
                    if (kvPos != std::string::npos)
                    {
                        std::string key = pair.substr(0, kvPos);
                        std::string value = pair.substr(kvPos + 1);
                        entity.properties[key] = value;
                    }
                }
            }

            if (entity.type != "Entrance")
            {
                room.entities.push_back(entity);
            }
            else
            {
                room.entrances.push_back(entity);
            }
        }
    }
};

#endif // ROOMLOADER_H
