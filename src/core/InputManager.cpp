#include "InputManager.h"

InputManager::InputManager()
{
    // Constructor
}

InputManager::~InputManager()
{
    // Destructor
}

// Publics

void InputManager::bind(Action action, sf::Keyboard::Key key)
{
    keyBindings[action] = key;
}

bool InputManager::isDown(Action action) const
{
    auto it = keyBindings.find(action);
    return it != keyBindings.end() &&
           sf::Keyboard::isKeyPressed(it->second);
}

bool InputManager::isPressed(Action action, sf::Keyboard::Key pressedKey) const
{
    auto it = keyBindings.find(action);
    return it != keyBindings.end() &&
           it->second == pressedKey;
}

std::string InputManager::getKeyName(Action action) const
{
    auto it = keyBindings.find(action);

    if (it == keyBindings.end())
    {
        return "?";
    }

    return keyToString(it->second);
}
