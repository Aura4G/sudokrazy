#ifndef __THEMES_HPP__
#define __THEMES_HPP__

#include <SFML/Graphics.hpp>

struct Theme {
    sf::Color bg1;
    sf::Color bg2;
    sf::Color bgClear;
    sf::Color text;
};

extern const Theme HOME_THEME;
extern const Theme EASY_THEME;
extern const Theme MEDIUM_THEME;
extern const Theme HARD_THEME;

#endif