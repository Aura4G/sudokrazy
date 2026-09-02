#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include <SFML/Graphics.hpp>

/**
 * SFML Button visualisation and Functionality
 * @author Aura4G (Aria Noroozi)
*/

/// @brief Contains all the colours needed for a button {bg color, hover color, text colour}
struct ButtonTheme {
    /// @brief when the mouse doesn't hover over a button
    sf::Color unhovered;
    /// @brief when the mouse hovers over a button
    sf::Color hovering;
    /// @brief The button's text colour
    sf::Color text;
};

/// @brief Specifies the frame type of the button
enum class ShapeType {Rectangle, Circle};

/// @brief A SFML button with appropriate logic and functionality
class Button {
public:
    /// @brief The shape the button frame will have
    ShapeType shapeType = ShapeType::Rectangle;
    
    /// @brief The visual bounds of the button
    sf::RectangleShape frame;

    /// @brief only used if shapeType == Circles
    sf::CircleShape circleFrame;
    
    /// @brief For text if needed in the button
    sf::Text text;

    /// @brief default constructor, necessary for button arrays
    Button();


    /** 
     * @brief Creates a specified SFML button
     * 
     * @param width
     * @param height
     * @param x
     * @param y
     * @param theme The button theme the button is using upon construction
     * @param targetText The text to display on the button
     * @param sharedFont The font object called from main for the button's text to use
     * @param shapeType The shape of the button frame
    */
    Button(float width, float height, float x, float y, const ButtonTheme& theme, const std::string& targetText, const std::string& font, ShapeType type = ShapeType::Rectangle);

    
    /**
     * @brief Changes the colour of the button frame
     * @param col The colour passed from the button theme
    */
    void setColor(const sf::Color& col);


    /**
     * @brief Set the text of the button, purely visual
     * @param newText The new text to be placed in the button
    */
    void setText(const std::string& newText);


    /**
     * @brief Changes the button theme and changes the text colour appropriately
     * @param newTheme The theme to replace the current theme
    */
    void setTheme(const ButtonTheme& newTheme);


    /**
     * @brief Changes the button sprite's texture
     * @param newTexture The texture to replace the button's current sprite
     * @param padding pixel distance from the frame's smallest dimension
    */
    void setTexture(const sf::Texture& newTexture, float padding = 10.f);


    /**
     * @brief Sets the button's frames, text and sprite to a another position on the window
     * @param newPos the new button position
    */
    void setPosition(const sf::Vector2f& newPos);

    /**
     * @brief Sets the origin for all sfml elements in the button
     * @param x
     * @param y
    */
    void setOrigin(float x, float y);


    /// @return The button width
    float getWidth();


    /// @return The button height
    float getHeight();


    /// @return A vector with the x and y position the button had upon construction
    sf::Vector2f getOriginalPos();


    /// @return A vector with the position the button currently has
    sf::Vector2f getPosition();


    /// @return the button theme being used by the button
    ButtonTheme getTheme();

    /// @return the text string of the button
    std::string getString();

    /// @return the texture currently being used by the button
    sf::Sprite getSprite();


    /**
     * @brief Used for validation and ensuring there aren't double/incorrect inputs
     * @return True if the button selected is active
    */
    bool isActive();


    /**
     * @param mousePos The mouse position relative to the window on the given frame 
     * @return True if the mouse is hovering over the button
    */
    bool isHovering(const sf::Vector2f& mousePos) const;


    /// @return whether or not the button has been hit during the game's events
    bool isHit() const;


    /**
     * @brief Changes the button frame cover according to mouse position and the button's action state
     * @param mousePos The mouse position relative to the window on the given frame 
    */
    void updateHover(const sf::Vector2f& mousePos);


    /**
     * @brief Calls all necessary draws for the text and frame
     * @param renderTexture The game render texture drawn on 
    */
    void display(sf::RenderTexture& renderTexture);


    /// @brief Sets active to true
    void activate();


    /// @brief Sets active to false
    void deactivate();


    /**
     * @brief changes the button's hit status
     * @param flag the new hit status
    */
    void setHit(bool flag);


    /**
     * @brief Sets a destination and move speed for the button to fulfil
     * @param destination The position vector where the button moves to
     * @param moveSpeed The speed at which the button moves 
    */
    void activateMovement(const sf::Vector2f& destination, float moveSpeed);

    /**
     * @brief Updates button position every frame, also ensuring smooth movement if movement is active
     * @param deltaTime Ensures button movement at a pace paralleling the window operation 
    */
    void update(float deltaTime);

    /// @brief Prevents button cloning and dangling texture pointers
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = default;
    Button& operator=(Button&&) = default;

private:
    /// @brief The width of the button frame
    float width;

    /// @brief The height of the button frame
    float height;

    /// @brief The x position of the button frame upon construction
    float x;

    /// @brief The y position of the button frame upon construction
    float y;

    /// @brief The button's move speed
    float moveSpeed = 150.f;

    /// @brief A flag to ensure valid inputs
    bool active = true;

    /// @brief A flag to determine whether the button is stationary or travelling using its moveTarget and movespeed
    bool isMoving = false;

    /// @brief A flag that determines whether a button has been previously clicked on,
    ///        used exclusively on grid buttons to validate a correct placement only once.
    bool hit = false;

    /// @brief The coordinates of the destination the button moves to if movement is active
    sf::Vector2f moveTarget;

    /// @brief The colour theme the button is currently using
    ButtonTheme theme;

    /// @brief For icons in the middle of the button
    sf::Sprite subject;

    /**
     * @brief Centers and Scales button text to fit in the button frame
     * @param text The button text
     * @param targetRect The target bounds for the text to fit in
     * @param padding However much distance from the edges of the bounds specified for the text to fit within
    */
    void fitTextInFrame(sf::Text& text, const sf::FloatRect& targetRect, float padding);

    /**
     * @brief Centers and Scales button text to fit in the circle button frame
     * @param text The button text
     * @param circle The target circle for the text to fit in
     * @param padding However much distance from the edges of the bounds specified for the text to fit within
    */
    void fitTextInFrame(sf::Text& text, const sf::CircleShape& circle, float padding);

    /**
     * @brief Centers and Scales button subject to fit in the button frame
     * @param subject The button texture
     * @param targetRect The target bounds for the texture to fit in
     * @param padding However much distance from the edges of the bounds specified for the texture to fit within
    */
    void fitSpriteInFrame(sf::Sprite& subject, const sf::FloatRect& targetRect, float padding);

    /**
     * @brief Centers and Scales button subject to fit in the circle button frame
     * @param subject The button subject
     * @param circle The target circle for the texture to fit in
     * @param padding However much distance from the edges of the bounds specified for the texture to fit within
    */
    void fitSpriteInFrame(sf::Sprite& subject, const sf::CircleShape& circle, float padding);
};

/// @brief Generic button, used also in default constructor
extern const ButtonTheme REGULAR_BUTTON;

/// @brief Used to identify user-affected cells in the SFML sudoku grid
extern const ButtonTheme INPUT_BUTTON;

/// @brief Button to select easy mode
extern const ButtonTheme EASY_BUTTON;

/// @brief Button to select medium mode
extern const ButtonTheme MEDIUM_BUTTON;

/// @brief Button to select hard mode
extern const ButtonTheme HARD_BUTTON;

/// @brief Button to select krazy mode
extern const ButtonTheme KRAZY_BUTTON;

/// @brief Exit/Close button
extern const ButtonTheme EXIT_BUTTON;

#endif