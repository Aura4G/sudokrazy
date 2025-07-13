#ifndef __SUDOKU_HPP__
#define __SUDOKU_HPP__

class Board {
public:
    Board();
    bool fillBoard();
    bool isSafe(int row, int col, int num);
    void printBoard();
    void removeNumbers(int difficulty = 2);
    int grid[9][9];
};

#endif
