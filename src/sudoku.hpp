#ifndef __SUDOKU_HPP__
#define __SUDOKU_HPP__

enum GameState {
    STATE_HOME,
    STATE_EASY,
    STATE_MEDIUM,
    STATE_HARD,
    STATE_KRAZY
};

class Board {
public:
    Board();
    bool fillBoard();
    bool isSafe(int row, int col, int num);
    friend bool operator== (Board& grid1, Board& grid2);
    void printBoard();
    void removeNumbers(GameState difficulty = STATE_MEDIUM);
    void setNumber(int y, int x, int number);
    void reset();
    int getNumber(int y, int x);

private:
    int grid[9][9];
};

#endif
