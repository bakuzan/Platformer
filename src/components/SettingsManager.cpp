#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>

#include "SettingsManager.h"

SettingsManager::SettingsManager()
{
    load();
}

// Publics

void SettingsManager::save()
{
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open())
    {
        std::cerr << "[SettingsManager] Could not open settings file for writing: "
                  << filename << "\n";
        return;
    }

    // Serialize settings as key=value pairs
    // file << "environmentType="
    //      << std::to_string(static_cast<int>(environmentType)) << "\n"
    //      << "difficulty="
    //      << std::to_string(static_cast<int>(difficulty)) << "\n";

    file.close();
}

void SettingsManager::reset()
{
    load();
}

// Privates

void SettingsManager::load()
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        // No user settings, just use code defaults
        return;
    }

    std::string line;
    std::unordered_map<std::string, std::string> kvStore;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') &&
            std::getline(iss, value))
        {
            kvStore[key] = value;
        }
    }

    // Apply settings
}