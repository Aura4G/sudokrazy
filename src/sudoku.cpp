#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include "sudoku.hpp"

Board::Board() {
    srand(time(0));
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            grid[y][x] = 0;
        }
    }
    fillBoard();
}

bool Board::isSafe(int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return false;
        }
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            if (grid[startRow + y][startCol + x] == num) {
                return false;
            }
        }
    }

    return true;
}

bool Board::fillBoard() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col] == 0) {
                std::vector<int> numbers = {1,2,3,4,5,6,7,8,9};
                std::random_shuffle(numbers.begin(), numbers.end());

                for (int num : numbers) {
                    if (isSafe(row, col, num)) {
                        grid[row][col] = num;

                        if (fillBoard()) {
                            return true;
                        }

                        grid[row][col] = 0;
                    }
                }

                return false;
            }
        }
    }

    return true;
}

void Board::removeNumbers(GameState difficulty) {
    int countdown = 0;

    if (difficulty == STATE_EASY) {
        countdown = rand() % 10 + 10;
    } else if (difficulty == STATE_MEDIUM) {
        countdown = rand() % 20 + 20;
    } else if (difficulty == STATE_HARD) {
        countdown = rand() % 15 + 40;
    }

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int random = rand() % 10;
            if (random < 2 && countdown > 0 && grid[y][x] != 0) {
                grid[y][x] = 0;
                countdown--;
            }
        }

        if (y == 8 && countdown != 0) {
            y = -1;
        }
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

bool operator== (Board& grid1, Board& grid2) {
    for (int i = 0; i < 81; i++) {
        if (grid1.getNumber(i/9, i%9) != grid2.getNumber(i/9, i%9)) {
            return false;
        }
    }
    return true;
}