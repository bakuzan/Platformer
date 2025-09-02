#include "Button.h"
#include "constants/Constants.h"

Button::Button(const std::string &name,
               const sf::Font &font,
               const std::string &label,
               sf::Vector2f position,
               std::function<void()> action)
    : name(name), action(std::move(action))
{
    // Config shape
    shape.setSize(sf::Vector2f(Constants::BUTTON_WIDTH, Constants::BUTTON_HEIGHT));
    shape.setFillColor(sf::Color(31, 31, 31)); // TODO make this customisable or even a texture
    shape.setPosition(position);

    // Configure button text
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(
        position.x + (shape.getSize().x - text.getGlobalBounds().width) / 2.f,
        position.y + (shape.getSize().y - text.getGlobalBounds().height) / 2.f);
}

Button::~Button()
{
    // Destructor
}

// Publics

void Button::render(sf::RenderWindow &window) const
{
    window.draw(shape);
    window.draw(text);
}

void Button::onHover(bool isHovered)
{
    text.setFillColor(isHovered ? sf::Color::Yellow : sf::Color::White);
}

bool Button::isMouseOver(sf::Vector2i mousePosition, const sf::RenderWindow &window) const
{
    sf::Vector2f windowPos = window.mapPixelToCoords(mousePosition);
    return shape.getGlobalBounds().contains(windowPos);
}

void Button::trigger() const
{
    action();
}

void Button::setPosition(sf::Vector2f position)
{
    shape.setPosition(position);
    text.setPosition(
        position.x + (shape.getSize().x - text.getGlobalBounds().width) / 2.f,
        position.y + (shape.getSize().y - text.getGlobalBounds().height) / 2.f);
}

const std::string Button::getName() const
{
    return name;
}