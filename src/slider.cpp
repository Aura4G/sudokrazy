#include <SFML/Graphics.hpp>
#include "slider.hpp"
#include "button.hpp"

Slider::Slider(float length, sf::Vector2f position, sf::Color color)
    : length(length), initialPosition(position), grip(15.f,15.f,0.f,0.f,MEDIUM_BUTTON,"","gameFont",ShapeType::Circle)
    {
        space.setPosition(position);
        space.setSize(sf::Vector2f(length, 10.f));
        space.setFillColor(color);

        grip.setOrigin(grip.getWidth(), grip.getWidth());
        grip.setPosition(sf::Vector2f(initialPosition.x, initialPosition.y + space.getSize().y/2));
    }

void Slider::display(sf::RenderWindow& window) {
    window.draw(space);
    grip.display(window);
}