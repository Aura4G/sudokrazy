#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>

#include "grid.hpp"
#include "sudoku.hpp"
#include "button.hpp"

Grid::Grid(sf::Vector2f position, float size, const sf::Font& sharedFont) {

    //Distance between each white block, and from the outer frame
    const float PADDING = 2.5f;
    back.setSize(sf::Vector2f(size + PADDING*2, size + PADDING*2));
    back.setFillColor(sf::Color::Black);
    back.setPosition(position - sf::Vector2f(PADDING/2.f, PADDING/2.f));

    //Adds to the x distance from the origin the next button should have
    float additiveX;
    //Adds to the y distance from the origin the next button should have
    float additiveY;

    //Iteratives 9x9 times, once for each block
    for (int i = 0; i < 81; i++) {
        additiveX = (i % 9) * (size/9.f);
        additiveY = static_cast<int>(i / 9) * (size/9.f);

        int y = static_cast<int>(i / 9);
        int x = i % 9;

        //extra x and y distance is added, to better visualise each individual 3x3

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
        if (playersBoard.getNumber(y,x) == 0) {
            input = ""; //visualises a removed number (a block the user can alter)
        } else {
            input = std::to_string(playersBoard.getNumber(y,x)); //this block doesn't get changed in the current game
        }

        //the position of the white block
        sf::Vector2f whiteBlockPos(additiveX + position.x + PADDING/2.f, additiveY + position.y + PADDING/2.f);
        Button block(size/9.f - PADDING*2.f, size/9.f - PADDING*2.f, whiteBlockPos.x, whiteBlockPos.y, REGULAR_BUTTON, input, sharedFont);
        if (playersBoard.getNumber(y,x) != 0) {
            block.deactivate(); //blocks that already have visual numbers in them do not get altered in the current game
        }

        //the block is added to the array
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
    //Used to access a particular cell from row and column..
    int counter = 0;
    // .. when iterating through each block 
    for (Button& block : whiteBlocks) {
        if (block.frame.getGlobalBounds().contains(mousePos)) { //if current block is clicked on..
            if (block.isActive()) { //.. and active ..
                //.. The number the player is using replaces the current contents of the block
                
                std::string newText = std::to_string(number);
                block.setText(newText);
                playersBoard.setNumber(counter / 9, counter % 9, number);
            }
        }
        counter++;
    }
}

void Grid::activate() {
    for (Button& block : whiteBlocks) {
        if (block.text.getString() == "") { //only allows button functionality if the block starts out as empty
            block.activate();
        }
    }
}

void Grid::deactivate() {
    for (Button& block : whiteBlocks) {
        block.deactivate();
    }
}

bool Grid::check() {
    return finalBoard == playersBoard;
}

void Grid::appropriate(GameState difficulty) {
    finalBoard.reset(); //The answer board gets a completely new sudoku solution
    playersBoard = finalBoard; //The player's board copies the new solution..
    playersBoard.removeNumbers(difficulty); //And randomly removes numbers from it

    //Activates and deactivates buttons accordingly, also changing their text contents and themes.
    for (int i = 0; i < 81; i++) {
        if (playersBoard.getNumber(i/9,i%9) == 0) {
            whiteBlocks[i].setText("");
            whiteBlocks[i].setTheme(INPUT_BUTTON);
        } else {
            whiteBlocks[i].setText(std::to_string(playersBoard.getNumber(i/9,i%9)));
            whiteBlocks[i].setTheme(REGULAR_BUTTON);
        }
    }
}


void Grid::krazyMode() {
    //the entire grid must be polled to see how many numbers are in the player's board

    //numbers on the board already that shouldn't be changed
    int setNumbers[9] = {9,9,9,9,9,9,9,9,9};

    //numbers on the board that the player should change to win
    int changeableNumbers[9] = {9,9,9,9,9,9,9,9,9};

    for (Button& button : whiteBlocks) {
        try {
            int number = std::stoi(button.text.getString().toAnsiString());
            // check if it's an active button
            if (button.isActive()) {
                if (number != 0) {
                    changeableNumbers[number-1]--;
                }
            } else {
                setNumbers[number-1]--; 
            }
        } catch (const std::exception& e) { //error handling (this shouldn't occur at all given the buttons used)
            std::cerr << "Error converting text to int: " << e.what() << std::endl;
        }
    }

    //The answer board shuffles to a random new solution
    finalBoard.reset();

    //The player board copies the new solution
    playersBoard = finalBoard;

    //And removes numbers from the grid to match the quantity of that number in the prior player grid
    
}