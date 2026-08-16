#ifndef __SUDOKU_HPP__
#define __SUDOKU_HPP__

/**
 * Sudoku Game Functions
 * @author Aura4G (Aria Noroozi)
*/

/// @brief Enum defining the current state of the game, for improved readability
enum GameState {
    STATE_HOME,
    STATE_EASY,
    STATE_MEDIUM,
    STATE_HARD,
    STATE_KRAZY,
    STATE_SETTINGS,
    STATE_SHOP
};

/// @brief Backend logic and structure of a sudoku board, with appropriate methods to fill and remove from the board
class Board {
public:
    /// @brief Default constructor, on its own, it fills the board with 0s
    Board();
    

    /**
     * @brief Creates a random complete sudoku board recursively
     * @return True, once a complete, valid sudoku board has been created
    */
    bool fillBoard();


    /**
     * @brief Verifies whether a particular number is valid in a select row and column
     * @param row The row on which the number is being queried
     * @param col The column on which the number is being queried
     * @param num The number (from 1-9) to be queried/placed on the board
     * @return True if it's safe to place, false if otherwise
    */
    bool isSafe(int row, int col, int num);


    /**
     * @brief Friend operator overload function for simplified comparison of two boards
     * @param board1
     * @param board2
     * @return True if every cell in board 1 mirrors that of board 2
    */
    friend bool operator== (Board& board1, Board& board2);


    /// @deprecated prints a visualisation of the sudoku board on the console (NOT USED)
    void printBoard();


    /**
     * Removes numbers from a complete board and replaces them with 0s
     * The quantity of numbers removed is dependent on the game state (difficulty) selected
     * 
     * EASY: 10-20 numbers removed
     * 
     * MEDIUM: 20-40 numbers removed
     * 
     * HARD: 40-55 numbers removed
     * 
     * KRAZY: 30-50 numbers removed
    */ 
    void removeNumbers();


    /**
     * @brief Replaces the number of a selected active cell in the grid with the number the player is using
     * @param y the column in the board/grid (the array in the array being used)
     * @param x the row in the board/grid (the integer cell in the selected array being used)
     * @param number the new number to replace the original number
    */
    void setNumber(int y, int x, int number);


    /// @brief Sets all the cells to 0, then refills the board
    void reset();


    /**
     * @brief Retrieves the number from an arbitrary cell in the grid
     * @param y the column in the grid
     * @param x the row in the grid
     * @return the number in that cell of the grid
    */
    int getNumber(int y, int x);


    /**
     * @brief Used for krazy mode. Imitates shuffling the board without removing any values from
     *        the previous player board 
     * 
     * @param changeToRemove the quantity of interactive numbers from 1-9 that the player has not filled in.
     * @param incorrect the quantity of panels the user has incorrectly entered, that must be replicated in the new grid
    */
    void selectiveRemoval(int (&changeToRemove)[9], std::vector<std::array<int, 2>>& incorrect);

private:
    /// @brief The 9x9 sudoku grid on which numbers are added and removed according to both player actions and game state selection
    int grid[9][9];
};

#endif
