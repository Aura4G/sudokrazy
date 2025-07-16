#ifndef __GRID_HPP__
#define __GRID_HPP__

#include <SFML/Graphics.hpp>
#include "sudoku.hpp"
#include "button.hpp"

class Grid {
public:
    Grid(sf::Vector2f position, float size, const sf::Font& sharedFont);
    bool check();
    void display(sf::RenderWindow& window);
    void update(float deltaTime);
    void updateHover(const sf::Vector2f& mousePos);
    void updateNumbers(const sf::Vector2f& mousePos, int number);
    void activate();
    void deactivate();
    void appropriate(GameState difficulty);

private:
    sf::RectangleShape back;
    Button whiteBlocks[81];
    Board finalBoard;
    Board playersBoard = finalBoard;
};

#endif