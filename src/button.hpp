#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include <SFML/Graphics.hpp>

struct ButtonTheme {
    sf::Color unhovered;
    sf::Color hovering;
    sf::Color text;
};

class Button {
public:
    sf::RectangleShape frame;
    sf::Text text;

    Button();
    Button(float width, float height, float x, float y, const ButtonTheme& theme, const std::string& targetText, const sf::Font& sharedFont);

    void setColor(const sf::Color& col);
    void setText(const std::string& newText);
    void setTheme(const ButtonTheme& newTheme);
    float getWidth();
    float getHeight();
    sf::Vector2f getOriginalPos();
    bool isActive();
    bool isHovering(const sf::Vector2f& mousePos) const;
    void updateHover(const sf::Vector2f& mousePos);
    void display(sf::RenderWindow& window);
    void activate();
    void deactivate();
    void activateMovement(const sf::Vector2f& destination, float moveSpeed);
    void update(float deltaTime);

private:
    float width;
    float height;
    float x;
    float y;
    float moveSpeed = 150.f;
    bool active = true;
    bool isMoving = false;
    sf::Vector2f moveTarget;
    ButtonTheme theme;

    void fitTextInFrame(sf::Text& text, const sf::FloatRect& targetRect, float padding);
};

extern const ButtonTheme REGULAR_BUTTON;
extern const ButtonTheme INPUT_BUTTON;
extern const ButtonTheme EASY_BUTTON;
extern const ButtonTheme MEDIUM_BUTTON;
extern const ButtonTheme HARD_BUTTON;
extern const ButtonTheme KRAZY_BUTTON;

#endif