#ifndef __THEMES_HPP__
#define __THEMES_HPP__

#include <SFML/Graphics.hpp>
#include "button.hpp"

/**
 * Theme Structure and every instance for this project
 * @author Aura4G (Aria Noroozi)
*/

//contains all the different colour objects needed for a unique colour theme
struct Theme {
    //used for panel1
    sf::Color bg1;
    //used for panel2
    sf::Color bg2;
    //used for panel3
    sf::Color bg3;
    //used for the native background
    sf::Color bgClear;
    //used for general text
    sf::Color text;
};

/**
 * Cleanly transitions game colour themes with the game state
 * @param colour the edited colour
 * @param targetColour the theme's end colour to reach
 * @param dt delta time
 * @return a colour object closer/identical to the targetColour
*/
sf::Color updateColour(sf::Color colour, sf::Color targetColour, float dt);

//For the home screen
extern const Theme HOME_THEME;

//For an easy game
extern const Theme EASY_THEME;

//For a medium game
extern const Theme MEDIUM_THEME;

//For a hard game
extern const Theme HARD_THEME;

//For krazy mode
extern const Theme KRAZY_THEME;

//For the settings menu
extern const Theme SETTINGS_THEME;

#endif