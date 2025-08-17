#ifndef __SLIDER_HPP__
#define __SLIDER_HPP__

#include <SFML/Graphics.hpp>
#include "button.hpp"

class Slider {
public:
    sf::RectangleShape space;
    Button grip;

    Slider(float length, sf::Vector2f position, sf::Color color);
    void setLength();
    void setColor();
    void activate();
    void deactivate();
    void display(sf::RenderWindow& window);
    void update(float deltaTime);

    float getLength();
    float getOriginalPos();
    bool isActive();

private:
    float length;
    sf::Vector2f initialPosition;
    bool active = true;
};

#endif