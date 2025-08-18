#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include "themes.hpp"

sf::Color updateColour(sf::Color colour, sf::Color targetColour, float dt) {
    auto stepTo = [&](sf::Uint8 cur, sf::Uint8 tgt) -> sf::Uint8 {
        if (cur == tgt) {
            return cur;
        }

        // Exponential approach factor
        const float K = 5.f; // responsiveness, higher = faster approach
        float a = 1.f - std::exp(-K * dt);

        int next = int(std::round(cur + a * (int(tgt) - int(cur))));

        return static_cast<sf::Uint8>(std::clamp(next, 0, 255));
    };

    colour.r = stepTo(colour.r, targetColour.r);
    colour.g = stepTo(colour.g, targetColour.g);
    colour.b = stepTo(colour.b, targetColour.b);
    
    return colour;
}

/* THEMES */
//Background and text colour themes
const Theme HOME_THEME =     {{32,32,224},   {16,16,192},   {61,216,227},  {56,0,127},   {0,0,48},    };
const Theme EASY_THEME =     {{0,192,0},     {64,224,64},   {168,229,130}, {12,96,0},    {0,64,0},    };
const Theme MEDIUM_THEME =   {{142,235,20},  {235,220,20},  {235,113,20},  {200,197,54}, {64,64,0},   };
const Theme HARD_THEME =     {{255,17,41},   {192,0,24},    {221,48,51},   {102,0,51},   {80,0,0},    };
const Theme KRAZY_THEME =    {{70,15,67},    {186,16,164},  {221,48,217},  {40,2,36},    {255,255,255}};
const Theme SETTINGS_THEME = {{255,255,255}, {224,224,224}, {192,192,192}, {0,0,0},      {0,0,0},     };