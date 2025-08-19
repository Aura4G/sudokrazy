#ifndef __SLIDER_HPP__
#define __SLIDER_HPP__

#include <SFML/Graphics.hpp>
#include "button.hpp"

/**
 * The Slider class allows for the creation of SFML-visualised sliders, used for a continuous selection
 * of values in a particular setting
 * 
 * @author Aura4G (Aria Noroozi)
*/

/**
 * An SFML Slider with appropriate functionality
*/
class Slider {
public:
    
    /**
     * Parameterized constructor for sliders with variable length, position and color
     * @param length the slider length
     * @param position the vector position on the window the start of the slider will take
     * @param defaultSetting the position on the slider line the grip initially takes, must be be
     * a float between 0 and 1
     * @param color the color of the line of the slider
    */
    Slider(float length, sf::Vector2f position, float defaultSetting, sf::Color color);


    /**
     * Change the length of the slider line
     * @param newLength the new length of the slider
     * @todo
    */
    void setLength(float newLength);


    /**
     * Change the colour of the slider line
     * @param colour the new colour
     * @todo
    */
    void setColor(sf::Color colour);


    /**
    * "Activates" the slider, allowing for user interactions to affect the slider
    * and any settings attributed to the slider
    * 
    * @todo
    */
    void activate();


    /**
    * "Deactivates" the slider, prohibiting user interactions from affecting the slider
    * and any settings attributed to the slider
    * @todo
    */
    void deactivate();


    //Draws the slider
    //TODO
    void display(sf::RenderWindow& window);


    //TODO
    void update(float deltaTime);


    /**
    * Handles the slider's button movement and bounds according to player-triggered events
    * @param event the polled event passed from main, where the window is being rendered
    * @param window the rendered window the game is running on 
    */
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);


    //@return The length of the slider line "space"
    float getLength();


    //@return The original vector position of the slider when initially created
    float getOriginalPos();


    //@return how far along the slider line the button is, in %
    float getPercentage();


    //@return The activity status of the slider
    bool isActive();

    //Allows for dynamic display of slider information
    void displayPercentage(std::string before, std::string after);

private:
    //The slider's line length
    float length;

    //The Slider Grip's default position
    float defaultSetting;

    //The slider line's position
    sf::Vector2f initialPosition;

    //The slider's activity flag
    bool active = true;

    //Active when the player is clicking on the slider grip
    bool isDragging = false;

    //The distance between the mouse and button when dragging the button
    sf::Vector2f dragOffset;

    //The line on which the grip button slides
    sf::RectangleShape line;

    //The button held by the user when adjusting the slider
    Button grip;

};

#endif