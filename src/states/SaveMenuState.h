#ifndef SAVEMENUSTATE_H
#define SAVEMENUSTATE_H

#include <string>

#include "states/SaveLoadMenuState.h"

class SaveMenuState : public SaveLoadMenuState
{
private:
    const std::string currentSpawnName;

protected:
    void onSlotAction(int slot) override;

public:
    SaveMenuState(GameData &data, StateManager &manager, sf::RenderWindow &window,
                  const std::string spawnName);
    ~SaveMenuState();
};

#endif // SAVEMENUSTATE_H
