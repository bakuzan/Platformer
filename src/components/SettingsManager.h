#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <string>

class SettingsManager
{
private:
    std::string filename = "settings.txt";

    // Data

private:
    SettingsManager();
    ~SettingsManager() = default;

    void load();

public:
    static SettingsManager &getInstance()
    {
        static SettingsManager instance;
        return instance;
    }

    void save();
    void reset();

    // Accessors and mutators
};

#endif // SETTINGSMANAGER_H
