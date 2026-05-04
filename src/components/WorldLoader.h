#ifndef WORLDLOADER_H
#define WORLDLOADER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

#include "data/WorldData.h"

class WorldLoader
{
public:
    static WorldData load(const std::string &filename)
    {
        WorldData world;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open world file: " + filename);
        }

        std::string line;
        WorldData::LevelInfo current;
        bool inSection = false;

        while (std::getline(file, line))
        {
            // Trim whitespace
            trim(line);
            if (line.empty())
            {
                continue;
            }

            // Section header: [01]
            if (line.front() == '[' && line.back() == ']')
            {
                // If we were already in a section, store the previous one
                if (inSection)
                {
                    world.levels[current.id] = current;
                    for (auto &r : current.rooms)
                        world.roomToLevel[r] = current.id;
                }

                current = WorldData::LevelInfo{};
                current.id = line.substr(1, line.size() - 2);
                inSection = true;
                continue;
            }

            // Key=value pairs
            auto eq = line.find('=');
            if (eq == std::string::npos)
            {
                continue;
            }

            std::string key = line.substr(0, eq);
            std::string value = line.substr(eq + 1);
            trim(key);
            trim(value);

            if (key == "name")
            {
                current.name = value;
            }
            else if (key == "rooms")
            {
                current.rooms = splitCommaList(value);
            }
        }

        // Store last section
        if (inSection)
        {
            world.levels[current.id] = current;

            for (auto &r : current.rooms)
            {
                world.roomToLevel[r] = current.id;
            }
        }

        return world;
    }

private:
    static void trim(std::string &s)
    {
        while (!s.empty() && std::isspace(s.front()))
        {
            s.erase(s.begin());
        }
        while (!s.empty() && std::isspace(s.back()))
        {
            s.pop_back();
        }
    }

    static std::vector<std::string> splitCommaList(const std::string &s)
    {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, ','))
        {
            trim(item);
            if (!item.empty())
            {
                out.push_back(item);
            }
        }

        return out;
    }
};

#endif // WORLDLOADER_H
