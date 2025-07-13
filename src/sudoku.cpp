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

void Board::removeNumbers(int difficulty) {
    if (difficulty < 1 || difficulty > 3) {
        std::cerr << "This number option is not allowed.\n";
        exit(-1);
    }

    int countdown = 0;

    if (difficulty == 1) {
        countdown = rand() % 10 + 10;
    } else if (difficulty == 2) {
        countdown = rand() % 20 + 20;
    } else if (difficulty == 3) {
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
