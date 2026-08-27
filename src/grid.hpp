#ifndef __GRID_HPP__
#define __GRID_HPP__

#include <SFML/Graphics.hpp>
#include <vector>
#include "sudoku.hpp"
#include "button.hpp"

/**
 * 9*9 sudoku grid SFML visualisation and functionality
 * 
 * The sudoku grid makes use of the button class for each of the 81 panels,
 * done to reinforce the modular and scalable nature behind the buttons and
 * how they can be applied to a variety of structures and objects
 * 
 * @author Aura4G (Aria Noroozi)
*/

/// @brief A natively drawn and interactive 9x9 grid, integrated with the backend Board object to create a working sudoku board
class Grid {
public:
    /**
     * @brief Constructor (may need to add default later)
     * @param position The position relative to the window for the grid to be placed
    */
    Grid(sf::Vector2f position, float size);


    /**
     * @brief Checks if its two Board objects are equal, indicating game completion
     * @return True if the two boards match
    */
    bool check();


    /**
     * @brief Draws the grid frame and all of the buttons
     * @param renderTexture The render texture being drawn on
    */
    void display(sf::RenderTexture& renderTexture);


    /**
     * @brief Iteratively calls the update function for every button in the grid
     * @param deltaTime
    */
    void update(float deltaTime);


    /**
     * @brief Iteratively calls the update-hover function for every button in the grid
     * @param mousePos Mouse position relative to the window in the current frame
    */
    void updateHover(const sf::Vector2f& mousePos);


    /**
     * Called when the window is clicked on, this function checks if a button has been clicked on
     * and is active, resulting in the number in said button and board position being changed to the number
     * the player is currently playing with
     *
     * @param mousePos Mouse position relative to the window in the current frame
     * @param number The new number to replace the current number in the selected panel of the grid IF it is visible and active
    */
    bool updateNumbers(const sf::Vector2f& mousePos, int number);


    /// @brief Darkens numbers on the grid that are equal to the player's selected number
    /// @param number The selected number
    void changeIndicator(int number);


    /// @brief Activates the grid and every player-input button on the grid
    void activate();

    /// @brief Deactivates the grid and every button on the grid
    void deactivate();

    /**
     * @brief Creates a new sudoku problem, removes numbers based on the difficulty,
     *        and visualises this new problem on the grid
    */
    void appropriate();


    /// @brief Triggers the board shuffles every 5 turns and visualises this change to the sfml grid
    void krazyMode();

    /**
     * @brief calculates the spendable currency the player earns for a completed game
     * @param score the score reached in the game
     * @param timeMS the time in seconds taken to beat the board
     * @return the points earnt
    */
    int calculatePoints(int score, float time);

    /// @brief Reveals a correct number on the sudoku grid
    void useHint();

    /// @return The number of turns taken in the sudoku game
    int getTurns();

    /// @return The status of eraser mode
    static bool getEraserMode();

    /**
     * @brief Sets the status of eraser mode
     * @param val The new status 
    */ 
    static void setEraserMode(bool val);

private:
    /// @brief The black frame of the sudoku grid
    sf::RectangleShape back;

    /// @brief The panels containing the numbers on the grid
    std::vector<Button> whiteBlocks;

    /// @brief A full, complete sudoku board
    Board finalBoard;

    /**
     * Another sudoku board, but with some of its numbers removed.
     * 
     * This board is given to the player, visualised on the grid when a new game starts.
     * 
     * Its solution matches that of the final board
    */
    Board playersBoard = finalBoard;


    /// @brief Indicates how many turns the user has taken to complete the grid 
    int turns = 0;

    /// @brief Flag to ensure a shuffle occurs only once every 5 turns
    bool shuffled = false;

    /// @brief Indicates how many correct placements the user has made in one game
    int correct = 0;

    /// @brief Indicates how many incorrect placements the user has made in one game
    int incorrect = 0;

    /// @brief flag to identify whether the player is adding or removing numbers from the board
    static bool eraser_mode;
};

#endif