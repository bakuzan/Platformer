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
    static RoomData loadFromFile(const std::string &filename)
    {
        RoomData room;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            throw std::runtime_error("RoomLoader: Failed to open room file: " + filename);
        }

        std::string line;
        bool inEntities = false;

        while (std::getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }

            if (line == "[Entities]")
            {
                inEntities = true;
                continue;
            }

            if (!inEntities)
            {
                room.tileGrid.push_back(line);
            }
            else
            {
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

                room.entities.push_back(entity);
            }
        }

        return room;
    }
};

#endif // ROOMLOADER_H
