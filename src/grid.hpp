#ifndef __GRID_HPP__
#define __GRID_HPP__

#include <SFML/Graphics.hpp>
#include "sudoku.hpp"

class Grid {
public:
    Grid(sf::Vector2f position, float size);
    void display(sf::RenderWindow& window);

private:
    sf::RectangleShape back;
    sf::RectangleShape whiteBlocks[9];
};

#endif