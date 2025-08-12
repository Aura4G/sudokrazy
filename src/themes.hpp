#ifndef __THEMES_HPP__
#define __THEMES_HPP__

#include <SFML/Graphics.hpp>

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

#endif