#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <vector>
#include <array>
#include "sudoku.hpp"
#include "main.hpp"

Board::Board() {
    //sets random seed
    srand(time(0));
    for (int y = 0; y < 9; y++) { //fills each cell of each array with 0
        for (int x = 0; x < 9; x++) {
            grid[y][x] = 0;
        }
    }
    fillBoard(); //actual values are added
}

bool Board::isSafe(int row, int col, int num) {
    //iteratively checks if another number in the row or column matches the new number
    for (int i = 0; i < 9; i++) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return false;
        }
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;

    //iteratively checks if another number in its 3x3 matches the new number
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            if (grid[startRow + y][startCol + x] == num) {
                return false;
            }
        }
    }

    //returns true if nothing matches
    return true;
}

bool Board::fillBoard() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col] == 0) {
                //each iteration starts with this vector defining all the possible numbers in the cell
                std::vector<int> numbers = {1,2,3,4,5,6,7,8,9};
                std::random_shuffle(numbers.begin(), numbers.end()); //shuffle to ensure the first number is not always selected

                for (int num : numbers) {
                    if (isSafe(row, col, num)) { //checks if the number isn't a repeat in its 3x3, row or column
                        grid[row][col] = num; //before adding the number to the grid

                        if (fillBoard()) { //if all numbers up to this point are valid, it's true and the next number can be added
                            return true;
                        }

                        grid[row][col] = 0; //otherwise the number can't be applied and the next number from the vector is tried
                    }
                }

                //if none can be applied, returns false
                return false;
            }
        }
    }

    return true;
}

void Board::removeNumbers() {
    //Determines the quantity of numbers to be removed
    int countdown = 0;

    if (stateFlag == STATE_EASY || stateFlag == STATE_HOME) {
        countdown = rand() % 10 + 10; //countdown between 10 and 20 to be removed
    } else if (stateFlag == STATE_MEDIUM) { //krazy currently removes as much as normal
        countdown = rand() % 20 + 20; //countdown between 20 and 40 to be removed
    } else if (stateFlag == STATE_HARD) {
        countdown = rand() % 15 + 40; //countdown between 40 and 55 to be removed
    } else if (stateFlag == STATE_KRAZY) {
        countdown = rand() % 20 + 30; //countdown between 30 and 50 to be removed;
    }

    //iterates through the whole grid

    int counter = 0;
    while (countdown != 0) {
        int random = rand() % 10;
        int y = (counter % 81) / 9;
        int x = counter % 9;
        //1/5 chance of a removal naturally occuring, if and only if the countdown quota has not been met
        //and the number in this cell hasn't already been removed
        if (random < 2 && countdown > 0 && grid[y][x] != 0) {
            grid[y][x] = 0; //removes the number
            countdown--; //and the quota is 1 number closer to being met
        }
        counter++;
    }
}

void Board::printBoard(){
    std::cout << "-------------------------------------" << std::endl;
    for (int y = 0; y < 9; y++) {
        std::cout << "|";
        for (int x = 0; x < 9; x++) {
            if (grid[y][x] != 0) {
                std::cout << " " << grid[y][x] << " |";
            } else {
                std::cout << "   |";
            }
        }
        std::cout << std::endl << "-------------------------------------" << std::endl;
    }
}

void Board::setNumber(int y, int x, int number) {
    grid[y][x] = number;
}

int Board::getNumber(int y, int x) {
    return grid[y][x];
}

void Board::reset() {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            grid[y][x] = 0;
        }
    }
    fillBoard();
}

bool operator== (Board& board1, Board& board2) {
    for (int i = 0; i < 81; i++) {
        if (board1.getNumber(i/9, i%9) != board2.getNumber(i/9, i%9)) { //checks two cells in equal positions are equal
            return false;
        }
    }
    //returns true (the two boards match) if the prior condition is not met at any point
    return true;
}

void Board::selectiveRemoval(int (&changeToRemove)[9], std::vector<std::array<int, 2>>& incorrect) {
    //iterates through the whole grid
    int counter = 0;
    int zeroArray[9] = {0,0,0,0,0,0,0,0,0};

    //loop to remove numbers from the grid that weren't entered in the prior grid
    while (!std::equal(std::begin(changeToRemove), std::end(changeToRemove), std::begin(zeroArray))) {
        int random = rand() % 10;
        int y = (counter % 81) / 9;
        int x = counter % 9;
        //1/5 chance of a removal naturally occuring, if and only if the countdown quota has not been met
        //and the number in this cell hasn't already been removed
        if (random < 2 && grid[y][x] != 0) {
            if (changeToRemove[grid[y][x]-1] > 0) {
                changeToRemove[grid[y][x]-1]--; //and the quota is 1 number closer to being met
                grid[y][x] = 0; //removes the number
            }
        }
        counter = (counter + 1) % 81;
    }

    counter = 0;
    int pointer = 0;

    //loop to replace correct remaining values in the grid with the mistakes the player has made
    while (pointer != incorrect.size()) {
        int y = (counter % 81) / 9;
        int x = counter % 9;

        if (grid[y][x] == incorrect[pointer][0]) {
            grid[y][x] = incorrect[pointer][1];
            pointer++;
        }
        counter = (counter + 1) % 81;
    }
}