#include <SFML/Graphics.hpp>
#include <cmath>
#include "grid.hpp"
#include "sudoku.hpp"

#define PYTHAGORAS(a,b) std::sqrt(a * a + b * b)

Grid::Grid(sf::Vector2f position, float size) {
    const float PADDING = 10.f;
    back.setSize(sf::Vector2f(size + PADDING, size + PADDING));
    back.setFillColor(sf::Color::Black);
    back.setPosition(position - sf::Vector2f(PADDING/2.f, PADDING/2.f));

    float additiveX;
    float additiveY;

    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            additiveX = 0.f;
        } else if (i % 3 == 1) {
            additiveX = size/3.f;
        } else if (i % 3 == 2) {
            additiveX = 2.f*(size/3.f);
        }

        if (i / 3 < 1) {
            additiveY = 0.f;
        } else if (i / 3 < 2) {
            additiveY = size/3.f; 
        } else if (i / 3 < 3) {
            additiveY = 2.f*size/3.f;
        }

        sf::Vector2f whiteBlockPos(additiveX + position.x + PADDING/2.f, additiveY + position.y + PADDING/2.f);
        sf::RectangleShape whiteBlock(sf::Vector2f(size/3.f-PADDING, size/3.f-PADDING));
        whiteBlock.setFillColor(sf::Color::White);
        whiteBlock.setPosition(whiteBlockPos);
        whiteBlocks[i] = whiteBlock;
    }
}

void Grid::display(sf::RenderWindow& window) {
    window.draw(back);
    for (int i = 0; i < 9; i++) {
        window.draw(whiteBlocks[i]);
    }
}