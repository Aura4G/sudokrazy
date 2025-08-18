#include <SFML/Graphics.hpp>
#include "slider.hpp"
#include "button.hpp"

Slider::Slider(float length, sf::Vector2f position, sf::Color color)
    : length(length), initialPosition(position), grip(15.f,15.f,0.f,0.f,MEDIUM_BUTTON,"","gameFont",ShapeType::Circle)
    {
        line.setPosition(position);
        line.setSize(sf::Vector2f(length, 10.f));
        line.setFillColor(color);

        grip.setOrigin(grip.getWidth(), grip.getWidth());
        grip.setPosition(sf::Vector2f(initialPosition.x, initialPosition.y + line.getSize().y/2));
    }

void Slider::display(sf::RenderWindow& window) {
    window.draw(line);
    grip.display(window);
}

void Slider::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    //The grip is currently being clicked on
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        if (grip.circleFrame.getGlobalBounds().contains(mousePos)) {
            isDragging = true;
            dragOffset = grip.circleFrame.getPosition() - mousePos;
        }
    }

    //The player lets go of the slider
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    }

    //Moves the grip with the mouse, by detecting mouse movement and identifying the dragging flag
    if (event.type == sf::Event::MouseMoved && isDragging) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
        grip.circleFrame.setPosition(sf::Vector2f(mousePos.x + dragOffset.x, initialPosition.y + line.getSize().y/2));

        //The slider's grip button cannot leave the bounds of the slider itself.
        sf::FloatRect bounds = line.getGlobalBounds();
        if (mousePos.x < bounds.left) {
            grip.circleFrame.setPosition(sf::Vector2f(bounds.left, initialPosition.y + line.getSize().y/2));
        } else if (mousePos.x > bounds.left + bounds.width) {
            grip.circleFrame.setPosition(sf::Vector2f(bounds.left + bounds.width, initialPosition.y + line.getSize().y/2));
        }
    }
}