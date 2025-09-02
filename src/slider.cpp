#include <SFML/Graphics.hpp>
#include <string>
#include "slider.hpp"
#include "button.hpp"
#include "resource_manager.hpp"
#include "main.hpp"

Slider::Slider(float length, sf::Vector2f position, float defaultSetting, sf::Color color)
    : length(length), initialPosition(position), defaultSetting(defaultSetting), grip(20.f,20.f,position.x,position.y,MEDIUM_BUTTON,"","homeFont",ShapeType::Circle)
    {
        line.setPosition(position);
        line.setSize(sf::Vector2f(length, 10.f));
        line.setFillColor(color);

        grip.setOrigin(grip.getWidth(), grip.getWidth());

        //Places the slider's grip button at the default position specified in the function call
        //This means that the slider's initial percentage is whatever position default gives the button
        grip.setPosition(sf::Vector2f(initialPosition.x + defaultSetting * length, initialPosition.y + line.getSize().y/2));
    }

void Slider::display(sf::RenderTexture& renderTexture) {
    renderTexture.draw(line);
    grip.display(renderTexture);
}

void Slider::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    //The grip is currently being clicked on
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && active) {
                
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}, gameView);
        if (grip.circleFrame.getGlobalBounds().contains(mousePos)) {
            isDragging = true;
            dragOffset = grip.getPosition() - mousePos;
        }
    }

    //The player lets go of the slider
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    }

    //Moves the grip with the mouse, by detecting mouse movement and identifying the dragging flag
    if (event.type == sf::Event::MouseMoved && isDragging) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y}, gameView);
        grip.setPosition(sf::Vector2f(mousePos.x + dragOffset.x, initialPosition.y + line.getSize().y/2));


        //The slider's grip button cannot leave the bounds of the slider itself.
        sf::FloatRect bounds = line.getGlobalBounds();
        if (mousePos.x < bounds.left) {
            grip.setPosition(sf::Vector2f(bounds.left, initialPosition.y + line.getSize().y/2));
        } else if (mousePos.x > bounds.left + bounds.width) {
            grip.setPosition(sf::Vector2f(bounds.left + bounds.width, initialPosition.y + line.getSize().y/2));
        }
    }

    if (isDragging) {
        grip.setColor(grip.getTheme().hovering);
    } else {
        grip.setColor(grip.getTheme().unhovered);
    }
}

float Slider::getPercentage() {
    sf::FloatRect bounds = line.getGlobalBounds();

    return ((grip.getPosition().x - bounds.left) / bounds.width) * 100;
}

void Slider::activate() {
    active = true;
}

void Slider::deactivate() {
    active = false;
}

void Slider::displayPercentage(std::string before = "", std::string after = "") {
    //Button display only needs integer information to be displayed; it's not complex
    int currentNum = static_cast<int>(getPercentage());

    std::string finalString = before + std::to_string(currentNum) + after;

    grip.setText(finalString);

    //The button's position must be set again to allow for text to move with the button
    grip.setPosition(grip.getPosition());
}

bool Slider::getDragging() {
    return isDragging;
}