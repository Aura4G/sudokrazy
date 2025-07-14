#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "grid.hpp"
#include "sudoku.hpp"
#include "button.hpp"

Grid::Grid(sf::Vector2f position, float size, const sf::Font& sharedFont) {
    const float PADDING = 2.5f;
    back.setSize(sf::Vector2f(size + PADDING*2, size + PADDING*2));
    back.setFillColor(sf::Color::Black);
    back.setPosition(position - sf::Vector2f(PADDING/2.f, PADDING/2.f));

    float additiveX;
    float additiveY;

    for (int i = 0; i < 81; i++) {
        additiveX = (i % 9) * (size/9.f);
        additiveY = static_cast<int>(i / 9) * (size/9.f);

        if (i % 9 > 2) {
            additiveX += PADDING;
        }
        if (i % 9 > 5) {
            additiveX += PADDING;
        }

        if (static_cast<int>(i / 9) > 2) {
            additiveY += PADDING;
        }
        if (static_cast<int>(i / 9) > 5) {
            additiveY += PADDING;
        }

        sf::Vector2f whiteBlockPos(additiveX + position.x + PADDING/2.f, additiveY + position.y + PADDING/2.f);
        Button block(size/9.f - PADDING*2.f, size/9.f - PADDING*2.f, whiteBlockPos.x, whiteBlockPos.y, REGULAR_BUTTON, "", sharedFont);
        whiteBlocks[i] = block;
    }
}

void Grid::display(sf::RenderWindow& window) {
    window.draw(back);
    for (int i = 0; i < 81; i++) {
        whiteBlocks[i].display(window);
    }
}

void Grid::update(float deltaTime) {
    for (int i = 0; i < 81; i++) {
        whiteBlocks[i].update(deltaTime);
    }
}

void Grid::updateHover(const sf::Vector2f& mousePos) {
    for (int i = 0; i < 81; i++) {
        whiteBlocks[i].updateHover(mousePos);
    }
}

void Grid::updateNumbers(const sf::Vector2f& mousePos, int number) {
    for (Button& button : whiteBlocks) {
        if (button.frame.getGlobalBounds().contains(mousePos)) {
            if (button.isActive()) {
                std::string newText = std::to_string(number);
                button.setText(newText);
            }
        }
    }
}