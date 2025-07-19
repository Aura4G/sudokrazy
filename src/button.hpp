#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include <SFML/Graphics.hpp>

//Contains all the colours needed for a button
struct ButtonTheme {
    //when the mouse doesn't hover over a button
    sf::Color unhovered;
    //when the mouse hovers over a button
    sf::Color hovering;
    //The button's text colour
    sf::Color text;
};

//A SFML button with appropriate logic and functionality
class Button {
public:
    //The visual bounds of the button
    sf::RectangleShape frame;
    //For text if needed in the button
    sf::Text text;

    //default constructor, necessary for button arrays
    Button();


    /** Creates a specified SFML button
    * @param width
    * @param height
    * @param x
    * @param y
    * @param theme The button theme the button is using upon construction
    * @param targetText The text to display on the button
    * @param sharedFont The font object called from main for the button's text to use
    */
    Button(float width, float height, float x, float y, const ButtonTheme& theme, const std::string& targetText, const sf::Font& sharedFont);


    /** Changes the colour of the button frame
    * @param col The colour passed from the button theme
    */
    void setColor(const sf::Color& col);


    /** Set the text of the button, purely visual
    * @param newText The new text to be placed in the button
    */
    void setText(const std::string& newText);


    /** Changes the button theme and changes the text colour appropriately
    * @param newTheme The theme to replace the current theme
    */
    void setTheme(const ButtonTheme& newTheme);


    /** @returns The button width */
    float getWidth();


    /** @returns The button height */
    float getHeight();


    /** @returns A vector with the x and y position the button had upon construction */
    sf::Vector2f getOriginalPos();


    /** @returns the button theme being used by the button */
    ButtonTheme getTheme();


    /** Used for validation and ensuring there aren't double/incorrect inputs
    * @returns True if the button selected is active
    */
    bool isActive();


    /**
    * @param mousePos The mouse position relative to the window on the given frame 
    * @returns True if the mouse is hovering over the button
    */
    bool isHovering(const sf::Vector2f& mousePos) const;


    /** Changes the button frame cover according to mouse position and the button's action state
    * @param mousePos The mouse position relative to the window on the given frame 
    */
    void updateHover(const sf::Vector2f& mousePos);


    /** Calls all necessary draws for the text and frame
    * @param window The game window drawn on 
    */
    void display(sf::RenderWindow& window);


    //Sets active to true
    void activate();


    //Sets active to false
    void deactivate();


    /** Sets a destination and move speed for the button to fulfil
    * @param destination The position vector where the button moves to
    * @param moveSpeed The speed at which the button moves 
    */
    void activateMovement(const sf::Vector2f& destination, float moveSpeed);

    /** Updates button position every frame, also ensuring smooth movement if movement is active
    * @param deltaTime Ensures button movement at a pace paralleling the window operation 
    */
    void update(float deltaTime);

private:
    //The width of the button frame
    float width;

    //The height of the button frame
    float height;

    //The x position of the button frame upon construction
    float x;

    //The y position of the button frame upon construction
    float y;

    //The button's move speed
    float moveSpeed = 150.f;

    //A flag to ensure valid inputs
    bool active = true;

    //A flag to determine whether the button is stationary or travelling using its moveTarget and movespeed
    bool isMoving = false;

    //The coordinates of the destination the button moves to if movement is active
    sf::Vector2f moveTarget;

    //The colour theme the button is currently using
    ButtonTheme theme;

    /** Centers and Scales button text to fit in the button frame
    * @param text The button text
    * @param targetRect The target bounds for the text to fit in
    * @param padding However much distance from the edges of the bounds specified for the text to fit within
    */
    void fitTextInFrame(sf::Text& text, const sf::FloatRect& targetRect, float padding);
};

//Generic button, used also in default constructor
extern const ButtonTheme REGULAR_BUTTON;

//Used to identify user-affected cells in the SFML sudoku grid
extern const ButtonTheme INPUT_BUTTON;

//Button to select easy mode
extern const ButtonTheme EASY_BUTTON;

//Button to select medium mode
extern const ButtonTheme MEDIUM_BUTTON;

//Button to select hard mode
extern const ButtonTheme HARD_BUTTON;

//Button to select krazy mode
extern const ButtonTheme KRAZY_BUTTON;

#endif