#ifndef LOADMENUSTATE_H
#define LOADMENUSTATE_H

#include <string>

#include "states/SaveLoadMenuState.h"

class LoadMenuState : public SaveLoadMenuState
{
protected:
    void onSlotAction(int slot) override;

public:
    LoadMenuState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~LoadMenuState();
};

#endif // LOADMENUSTATE_H
