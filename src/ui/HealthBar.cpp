#include "constants/Constants.h"
#include "HealthBar.h"

HealthBar::HealthBar(sf::Vector2f size, float outlineInset)
    : dimensions(size),
      inset(outlineInset)
{
    background.setSize(dimensions);
    background.setFillColor(Constants::uiBackgroundColour);

    foreground.setSize({dimensions.x - (inset * 2.f), dimensions.y - (inset * 2.f)});
}

void HealthBar::update(int health, int maxHealth, sf::Vector2f position)
{
    if (maxHealth <= 0)
    {
        return;
    }

    float ratio = std::max(0.0f, static_cast<float>(health) / maxHealth);

    background.setPosition(position);
    foreground.setPosition({position.x + inset, position.y + inset});

    foreground.setSize({(dimensions.x - (inset * 2.f)) * ratio, dimensions.y - (inset * 2.f)});

    if (ratio > 0.7f)
    {
        foreground.setFillColor(sf::Color::Green);
    }
    else if (ratio > 0.4f)
    {
        foreground.setFillColor(sf::Color::Yellow);
    }
    else if (ratio > 0.2f)
    {
        foreground.setFillColor(sf::Color(255, 165, 0));
    }
    else
    {
        foreground.setFillColor(sf::Color::Red);
    }
}

void HealthBar::render(sf::RenderWindow &window) const
{
    window.draw(background);
    window.draw(foreground);
}
