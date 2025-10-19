#include <memory>
#include <format>

#include "constants/Constants.h"
#include "utils/InputUtils.h"
#include "SaveMenuState.h"
#include "SaveMenuState.h"

SaveMenuState::SaveMenuState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win)
{
    sf::Vector2f viewSize = saveView.getSize();
    sf::Vector2f center(saveView.getCenter());
    window.setView(saveView);

    // Configure background
    background.setSize(sf::Vector2f(viewSize.x - 20.0f, viewSize.y - 20.0f));
    background.setFillColor(sf::Color(0, 0, 0, 150));

    // Configure text
    saveText.setFont(gameData.gameFont);
    saveText.setString("Saves");
    saveText.setCharacterSize(36);
    saveText.setFillColor(sf::Color::White);

    // Slots
    for (int i = 0; i < 3; ++i)
    {
        slots[i].box.setSize({400.f, 60.f});
        slots[i].box.setFillColor(sf::Color(50, 50, 50, 200));
        slots[i].box.setOutlineThickness(2.f);
        slots[i].box.setOutlineColor(sf::Color::White);

        slots[i].label.setFont(gameData.gameFont);
        slots[i].label.setCharacterSize(24);
        slots[i].label.setFillColor(sf::Color::White);
    }

    // Confirmation overlay
    confirmText.setFont(gameData.gameFont);
    confirmText.setString("Are you sure you want to save?");
    confirmText.setCharacterSize(28);
    confirmText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = confirmText.getLocalBounds();
    float paddingX = 20.f;
    float paddingY = 80.f;
    confirmBox.setSize({textBounds.width + paddingX * 2.f,
                        textBounds.height + paddingY * 2.f});
    confirmBox.setFillColor(sf::Color(0, 0, 0, 220));
    confirmBox.setOutlineThickness(3.f);
    confirmBox.setOutlineColor(sf::Color::White);

    confirmButtons[0].setFont(gameData.gameFont);
    confirmButtons[0].setString("Yes");
    confirmButtons[0].setCharacterSize(24);

    confirmButtons[1].setFont(gameData.gameFont);
    confirmButtons[1].setString("No");
    confirmButtons[1].setCharacterSize(24);

    highlightedSlot = 0;
    highlightedConfirm = 1; // default = No

    refreshSlots();
    updateMenuItemPositions();
}

SaveMenuState::~SaveMenuState()
{
    // Destructor
}

// Publics

void SaveMenuState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Escape}))
        {
            if (showingConfirm)
            {
                showingConfirm = false; // close overlay
            }
            else
            {
                stateManager.popState(); // Resume the game by returning to GameState
            }
        }

        if (!showingConfirm)
        {
            if (event.key.code == sf::Keyboard::W ||
                event.key.code == sf::Keyboard::Up)
            {
                moveHighlight(-1);
            }

            if (event.key.code == sf::Keyboard::S ||
                event.key.code == sf::Keyboard::Down)
            {
                moveHighlight(+1);
            }

            if (event.key.code == sf::Keyboard::Enter)
            {
                selectSlot();
            }
        }
        else
        {
            if (event.key.code == sf::Keyboard::A ||
                event.key.code == sf::Keyboard::Left)
            {
                handleConfirmInput(-1);
            }

            if (event.key.code == sf::Keyboard::D ||
                event.key.code == sf::Keyboard::Right)
            {
                handleConfirmInput(+1);
            }

            if (event.key.code == sf::Keyboard::Enter)
            {
                confirmSelection();
            }
        }
    }

    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y},
                                                        saveView);

        if (!showingConfirm)
        {
            int idx = hitTestSlots(mousePos);
            if (idx != -1)
            {
                highlightedSlot = idx;
            }
        }
        else
        {
            int idx = hitTestConfirm(mousePos);
            if (idx != -1)
            {
                highlightedConfirm = idx;
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y},
                                                        saveView);

        if (!showingConfirm)
        {
            int idx = hitTestSlots(mousePos);
            if (idx != -1)
            {
                highlightedSlot = idx;
                selectSlot();
            }
        }
        else
        {
            int idx = hitTestConfirm(mousePos);
            if (idx != -1)
            {
                highlightedConfirm = idx;
                confirmSelection();
            }
        }
    }
}

void SaveMenuState::handleWindowResize(sf::Vector2u newSize)
{
    (void)newSize;
    updateMenuItemPositions();
}

void SaveMenuState::update(sf::Time deltaTime)
{
    (void)deltaTime;

    gameData.audioManager.cleanupSounds();
}

void SaveMenuState::render()
{
    window.setView(saveView);
    window.draw(background);
    window.draw(saveText);

    // Draw slots
    for (int i = 0; i < 3; ++i)
    {
        slots[i].box.setOutlineColor(i == highlightedSlot
                                         ? sf::Color::Yellow
                                         : sf::Color::White);
        window.draw(slots[i].box);
        window.draw(slots[i].label);
    }

    // Confirmation overlay
    if (showingConfirm)
    {
        window.draw(confirmBox);
        window.draw(confirmText);

        for (int i = 0; i < 2; ++i)
        {
            confirmButtons[i].setFillColor(i == highlightedConfirm
                                               ? sf::Color::Yellow
                                               : sf::Color::White);
            window.draw(confirmButtons[i]);
        }
    }
}

// Privates
void SaveMenuState::updateMenuItemPositions()
{
    sf::Vector2f viewCenter = saveView.getCenter();
    sf::Vector2f backgroundSize = background.getSize();

    background.setPosition(
        viewCenter.x - backgroundSize.x / 2.f,
        viewCenter.y - backgroundSize.y / 2.f);

    saveText.setPosition(
        background.getPosition().x + 10.f,
        background.getPosition().y + 20.f);

    // Slots stacked vertically
    float startY = saveText.getPosition().y + 80.f;
    for (int i = 0; i < 3; ++i)
    {
        slots[i].box.setPosition(viewCenter.x - 200.f, startY + i * 80.f);
        slots[i].label.setPosition(slots[i].box.getPosition().x + 20.f,
                                   slots[i].box.getPosition().y + 15.f);
    }

    // Confirmation overlay centered
    confirmBox.setPosition(viewCenter.x - confirmBox.getSize().x / 2.f,
                           viewCenter.y - confirmBox.getSize().y / 2.f);

    sf::FloatRect textBounds = confirmText.getLocalBounds();
    confirmText.setPosition(
        confirmBox.getPosition().x + (confirmBox.getSize().x - textBounds.width) / 2.f - textBounds.left,
        confirmBox.getPosition().y + 20.f);

    // --- Layout confirm buttons cleanly ---
    float buttonGap = 40.f;
    sf::FloatRect yesBounds = confirmButtons[0].getLocalBounds();
    sf::FloatRect noBounds = confirmButtons[1].getLocalBounds();
    float totalWidth = yesBounds.width + noBounds.width + buttonGap;
    float buttonY = confirmBox.getPosition().y + confirmBox.getSize().y - 60.f;
    float startX = confirmBox.getPosition().x + (confirmBox.getSize().x - totalWidth) / 2.f;

    confirmButtons[0].setPosition(
        startX - yesBounds.left,
        buttonY - yesBounds.top);

    confirmButtons[1].setPosition(
        confirmButtons[0].getPosition().x + yesBounds.width + buttonGap - noBounds.left,
        buttonY - noBounds.top);
}

void SaveMenuState::refreshSlots()
{
    // TODO: query save manager
    // For now, dummy data
    slots[0].occupied = true;
    slots[0].label.setString("Save 1 - 12:34");

    slots[1].occupied = true;
    slots[1].label.setString("Save 2 - 11:20");

    slots[2].occupied = false;
    slots[2].label.setString("Empty Slot");
}

void SaveMenuState::moveHighlight(int dir)
{
    highlightedSlot = (highlightedSlot + dir + 3) % 3;
}

void SaveMenuState::selectSlot()
{
    if (!slots[highlightedSlot].occupied)
    {
        // TODO: call save manager directly
        stateManager.popState();
    }
    else
    {
        showConfirmation();
    }
}

void SaveMenuState::showConfirmation()
{
    showingConfirm = true;
    highlightedConfirm = 1;
}

void SaveMenuState::handleConfirmInput(int dir)
{
    highlightedConfirm = (highlightedConfirm + dir + 2) % 2;
}

void SaveMenuState::confirmSelection()
{
    if (highlightedConfirm == 0)
    {
        // Yes → overwrite
        // TODO: call save manager overwrite
        stateManager.popState();
    }
    else
    {
        // No → close overlay
        showingConfirm = false;
    }
}

int SaveMenuState::hitTestSlots(sf::Vector2f mousePos)
{
    for (int i = 0; i < 3; ++i)
    {
        if (slots[i].box.getGlobalBounds().contains(mousePos))
        {
            return i;
        }
    }

    return -1;
}

int SaveMenuState::hitTestConfirm(sf::Vector2f mousePos)
{
    for (int i = 0; i < 2; ++i)
    {
        if (confirmButtons[i].getGlobalBounds().contains(mousePos))
        {
            return i;
        }
    }

    return -1;
}
