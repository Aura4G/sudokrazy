#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>

#include "grid.hpp"
#include "sudoku.hpp"
#include "button.hpp"

Grid::Grid(sf::Vector2f position, float size, const sf::Font& sharedFont) {
    Board finalBoard;
    Board playersBoard = finalBoard;
    playersBoard.removeNumbers();

    const float PADDING = 2.5f;
    back.setSize(sf::Vector2f(size + PADDING*2, size + PADDING*2));
    back.setFillColor(sf::Color::Black);
    back.setPosition(position - sf::Vector2f(PADDING/2.f, PADDING/2.f));

    float additiveX;
    float additiveY;

    for (int i = 0; i < 81; i++) {
        additiveX = (i % 9) * (size/9.f);
        additiveY = static_cast<int>(i / 9) * (size/9.f);

        int y = static_cast<int>(i / 9);
        int x = i % 9;

        if (x > 2) {
            additiveX += PADDING;
        }
        if (x > 5) {
            additiveX += PADDING;
        }

        if (y > 2) {
            additiveY += PADDING;
        }
        if (y > 5) {
            additiveY += PADDING;
        }

        std::string input;
        if (playersBoard.grid[y][x] == 0) {
            input = "";
        } else {
            input = std::to_string(playersBoard.grid[y][x]);
        }

        sf::Vector2f whiteBlockPos(additiveX + position.x + PADDING/2.f, additiveY + position.y + PADDING/2.f);
        Button block(size/9.f - PADDING*2.f, size/9.f - PADDING*2.f, whiteBlockPos.x, whiteBlockPos.y, REGULAR_BUTTON, input, sharedFont);
        if (playersBoard.grid[y][x] != 0) {
            block.deactivate();
        }
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
    for (Button& block : whiteBlocks) {
        block.update(deltaTime);
    }
}

void Grid::updateHover(const sf::Vector2f& mousePos) {
    for (Button& block : whiteBlocks) {
        if (block.isActive()) {
            block.updateHover(mousePos);
        }
    }
}

void Grid::updateNumbers(const sf::Vector2f& mousePos, int number) {
    for (Button& block : whiteBlocks) {
        if (block.frame.getGlobalBounds().contains(mousePos)) {
            if (block.isActive()) {
                std::string newText = std::to_string(number);
                block.setText(newText);
            }
        }
    }
}

void Grid::activate() {
    for (Button& block : whiteBlocks) {
        if (block.text.getString() == "") {
            block.activate();
        }
    }
}

void Grid::deactivate() {
    for (Button& block : whiteBlocks) {
        block.deactivate();
    }
}