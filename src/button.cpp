#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "button.hpp"
#include "resource_manager.hpp"

//Button Colour Themes

const ButtonTheme REGULAR_BUTTON = {{255,255,255},{127,127,127},{0,0,0}};
const ButtonTheme INPUT_BUTTON =   {{255,255,255},{255,255,0},  {25,151,230}};
const ButtonTheme EASY_BUTTON =    {{0,255,0},    {0,127,0},    {0,64,0}};
const ButtonTheme MEDIUM_BUTTON =  {{255,255,0},  {127,127,0},  {64,64,0}};
const ButtonTheme HARD_BUTTON =    {{255,0,0},    {127,0,0},    {80,0,0}};
const ButtonTheme KRAZY_BUTTON =   {{169,0,194},  {75,0,86},    {46,0,53}};
const ButtonTheme EXIT_BUTTON =    {{32,32,32},   {0,0,0},      {144,0,32}};

//Method declarations for Button class

Button::Button() {
    width = 0;
    height = 0;
    x = 0;
    y = 0;
    theme = REGULAR_BUTTON;
}

Button::Button(float width, float height, float x, float y, const ButtonTheme& theme, const std::string& targetText, const sf::Font& sharedFont, ShapeType type)
    : width(width), height(height), x(x), y(y), theme(theme), shapeType(type)
    {
        //button frame formatting
        if (shapeType == ShapeType::Rectangle) {
            frame.setPosition(x,y);
            frame.setSize(sf::Vector2f(width, height));
        } else {
            circleFrame.setRadius(width);
            circleFrame.setOrigin(width,width);
            circleFrame.setPosition(x,y);
        }

        //button text formatting
        text.setString(targetText);
        text.setFont(sharedFont);
        text.setCharacterSize(48);
        text.setFillColor(theme.text);

        subject.setTexture(ResourceManager::getTexture("placeholder"));

        if (shapeType == ShapeType::Rectangle) {
            fitTextInFrame(text, frame.getGlobalBounds(), 10.0f);
            fitSpriteInFrame(subject, frame.getGlobalBounds(), 20.0f);
        } else {
            fitTextInFrame(text, circleFrame, 10.0f);
            fitSpriteInFrame(subject, circleFrame, 20.0f);
        }
    }

void Button::setColor(const sf::Color& col) {
    if (shapeType == ShapeType::Rectangle) {
        frame.setFillColor(col);
    } else {
        circleFrame.setFillColor(col);
    }
}

void Button::setText(const std::string& newText) {
    text.setString(newText);
    //The fit text function must be called again when introducing different text
    if (shapeType == ShapeType::Rectangle) {
        fitTextInFrame(text, frame.getGlobalBounds(), 10.0f);
    } else {
        fitTextInFrame(text, circleFrame, 10.0f);
    }
}

void Button::setTheme(const ButtonTheme& newTheme) {
    theme = newTheme;
    //text colour isn't updated each frame, so it must be updated here
    text.setFillColor(theme.text);
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

ButtonTheme Button::getTheme() {
    return theme;
}

bool Button::isActive() {
    return active;
}

bool Button::isHovering(const sf::Vector2f& mousePos) const {
    if (shapeType == ShapeType::Rectangle) {
        return frame.getGlobalBounds().contains(mousePos);
    } else {
        sf::Vector2f center = circleFrame.getPosition();
        float radius = circleFrame.getRadius();
        float dx = mousePos.x - center.x;
        float dy = mousePos.y - center.y;
        return (dx * dx + dy * dy) <= (radius * radius);
    }
}

void Button::updateHover(const sf::Vector2f& mousePos) {
    if (isHovering(mousePos)) {
        setColor(theme.hovering); //hover colour if hovering
    } else {
        setColor(theme.unhovered); //regular colour otherwise
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

void Button::fitTextInFrame(sf::Text& text, const sf::CircleShape& circle, float padding = 10.f) {
    // Get circle bounds
    sf::FloatRect circleBounds = circle.getGlobalBounds();

    //get bounds of the text
    sf::FloatRect textBounds = text.getLocalBounds();

    //get available width and height from button frame
    float availableWidth = circleBounds.width - padding * 2;
    float availableHeight = circleBounds.height - padding * 2;

    //Calculate a scale factor from our available widths and heights
    float scaleX = availableWidth / textBounds.width;
    float scaleY = availableHeight / textBounds.height;
    float scale = std::min(scaleX, scaleY); //choose the minimum between the two scales to ensure it fits in the bounds

    //Apply scale
    text.setScale(scale,scale);

    //Center the origin of the scaled text
    textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(x, y);
}

void Button::fitSpriteInFrame(sf::Sprite& subject, const sf::FloatRect& targetRect, float padding = 10.f) {
    //get bounds of the sprite
    sf::FloatRect subjectBounds = subject.getLocalBounds();

    //get available width and height from button frame
    float availableWidth = targetRect.width - 2 * padding;
    float availableHeight = targetRect.height - 2 * padding;

    //Calculate a scale factor from our available widths and heights
    float scaleX = availableWidth / subjectBounds.width;
    float scaleY = availableHeight / subjectBounds.height;
    float scale = std::min(scaleX, scaleY); //choose the minimum between the two scales to ensure it fits in the bounds

    //Apply scale
    subject.setScale(scale,scale);

    //Center the origin of the scaled sprite
    subjectBounds = subject.getLocalBounds();
    subject.setOrigin(subjectBounds.left + subjectBounds.width / 2.0f, subjectBounds.top + subjectBounds.height / 2.0f);
    subject.setPosition(x + width / 2.0f, y + height / 2.0f);
}

void Button::fitSpriteInFrame(sf::Sprite& subject, const sf::CircleShape& circle, float padding = 10.f) {
    // Get circle bounds
    sf::FloatRect circleBounds = circle.getGlobalBounds();

    //get bounds of the sprite
    sf::FloatRect subjectBounds = subject.getLocalBounds();

    //get available width and height from button frame
    float availableWidth = circleBounds.width - padding * 2;
    float availableHeight = circleBounds.height - padding * 2;

    //Calculate a scale factor from our available widths and heights
    float scaleX = availableWidth / subjectBounds.width;
    float scaleY = availableHeight / subjectBounds.height;
    float scale = std::min(scaleX, scaleY); //choose the minimum between the two scales to ensure it fits in the bounds

    //Apply scale
    subject.setScale(scale,scale);

    //Center the origin of the scaled sprite
    subjectBounds = subject.getLocalBounds();
    subject.setOrigin(subjectBounds.left + subjectBounds.width / 2.0f, subjectBounds.top + subjectBounds.height / 2.0f);
    subject.setPosition(x, y);
}

void Button::display(sf::RenderWindow& window) {
    if (shapeType == ShapeType::Rectangle) {
        window.draw(frame);
    } else {
        window.draw(circleFrame);
    }

    if (subject.getTexture() != &ResourceManager::getTexture("placeholder")) {
        window.draw(subject);
    }

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
    isMoving = true; //is moving is set to true
}

void Button::update(float deltaTime) {
    if (!isMoving) return; //if the button is not expected to move, the function is negated for this frame

    sf::Vector2f currentPos = frame.getPosition();
    sf::Vector2f direction = moveTarget - currentPos;

    //Euclidean approach
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance < 1.f) {
        isMoving = false; //set back to false once the destination has been reached.
        return;
    }

    sf::Vector2f movement = (direction / distance) * moveSpeed * deltaTime;
    //Clamp movement
    if (std::abs(movement.x) > std::abs(direction.x)) movement.x = direction.x;
    if (std::abs(movement.y) > std::abs(direction.y)) movement.y = direction.y;

    frame.move(movement);
    circleFrame.move(movement);
    subject.move(movement);
    text.move(movement);
}