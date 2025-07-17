#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "button.hpp"

//Button Colour Themes
const ButtonTheme REGULAR_BUTTON = {{255,255,255},{127,127,127},{0,0,0}};
const ButtonTheme INPUT_BUTTON =   {{255,255,255},{127,127,127},{25,151,230}};
const ButtonTheme EASY_BUTTON =    {{0,255,0},    {0,127,0},    {0,64,0}};
const ButtonTheme MEDIUM_BUTTON =  {{255,255,0},  {127,127,0},  {64,64,0}};
const ButtonTheme HARD_BUTTON =    {{255,0,0},    {127,0,0},    {80,0,0}};
const ButtonTheme KRAZY_BUTTON =   {{169,0,194},  {75,0,86},    {46,0,53}};

//Method declarations for Button class
Button::Button() {
    width = 0;
    height = 0;
    x = 0;
    y = 0;
    theme = REGULAR_BUTTON;
}

Button::Button(float width, float height, float x, float y, const ButtonTheme& theme, const std::string& targetText, const sf::Font& sharedFont)
    : width(width), height(height), x(x), y(y), theme(theme)
    {
        //button frame formatting
        frame.setPosition(x,y);
        frame.setSize(sf::Vector2f(width, height));

        //button text formatting
        text.setString(targetText);
        text.setFont(sharedFont);
        text.setCharacterSize(48);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        text.setPosition(x + width / 2.0f, y + height / 2.0f);
        text.setColor(theme.text);
        fitTextInFrame(text, frame.getGlobalBounds(), 10.0f);
    }

void Button::setColor(const sf::Color& col) {
    frame.setFillColor(col);
}

void Button::setText(const std::string& newText) {
    text.setString(newText);
    fitTextInFrame(text, frame.getGlobalBounds(), 10.0f);
}

void Button::setTheme(const ButtonTheme& newTheme) {
    theme = newTheme;
    text.setColor(theme.text);
}

float Button::getWidth() {
    return width;
}

float Button::getHeight() {
    return height;
}

sf::Vector2f Button::getOriginalPos() {
    return sf::Vector2f(x,y);
}

bool Button::isActive() {
    return active;
}

bool Button::isHovering(const sf::Vector2f& mousePos) const {
    return frame.getGlobalBounds().contains(mousePos);
}

void Button::updateHover(const sf::Vector2f& mousePos) {
    if (isHovering(mousePos)) {
        setColor(theme.hovering);
    } else {
        setColor(theme.unhovered);
    }
}

void Button::fitTextInFrame(sf::Text& text, const sf::FloatRect& targetRect, float padding = 10.f) {
    //get bounds of the text
    sf::FloatRect textBounds = text.getLocalBounds();

    //get available width and height from button frame
    float availableWidth = targetRect.width - 2 * padding;
    float availableHeight = targetRect.height - 2 * padding;

    //Calculate a scale factor from our available widths and heights
    float scaleX = availableWidth / textBounds.width;
    float scaleY = availableHeight / textBounds.height;
    float scale = std::min(scaleX, scaleY); //choose the minimum between the two scales to ensure it fits in the bounds

    //Apply scale
    text.setScale(scale,scale);

    //Center the origin of the scaled text
    textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(x + width / 2.0f, y + height / 2.0f);
}

void Button::display(sf::RenderWindow& window) {
    window.draw(frame);
    window.draw(text);
}

void Button::activate() {
    active = true;
}

void Button::deactivate() {
    active = false;
}

void Button::activateMovement(const sf::Vector2f& destination, float moveSpeed = 150.f) {
    this->moveSpeed = moveSpeed;
    moveTarget = destination;
    isMoving = true;
}

void Button::update(float deltaTime) {
    if (!isMoving) return;

    sf::Vector2f currentPos = frame.getPosition();
    sf::Vector2f direction = moveTarget - currentPos;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance < 1.f) {
        isMoving = false;
        return;
    }

    sf::Vector2f movement = (direction / distance) * moveSpeed * deltaTime;
    //Clamp movement
    if (std::abs(movement.x) > std::abs(direction.x)) movement.x = direction.x;
    if (std::abs(movement.y) > std::abs(direction.y)) movement.y = direction.y;

    frame.move(movement);
    text.move(movement);
}