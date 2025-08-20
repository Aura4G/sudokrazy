#ifndef __GRID_HPP__
#define __GRID_HPP__

#include <SFML/Graphics.hpp>
#include <vector>
#include "sudoku.hpp"
#include "button.hpp"

/**
 * 9*9 sudoku grid SFML visualisation and functionality
 * @author Aura4G (Aria Noroozi)
*/

//A natively drawn and interactive 9x9 grid, integrated with the backend Board object to create a working sudoku board
class Grid {
public:
    /** Constructor (may need to add default later)
    * @param position The position relative to the window for the grid to be placed
    */
    Grid(sf::Vector2f position, float size);


    /** Checks if its two Board objects are equal, indicating game completion
    * @return True if the two boards match
    */
    bool check();


    /** Draws the grid frame and all of the buttons
    * @param renderTexture The render texture being drawn on
    */
    void display(sf::RenderTexture& renderTexture);


    /** Iteratively calls the update function for every button in the grid
    * @param deltaTime
    */
    void update(float deltaTime);


    /** Iteratively calls the update-hover function for every button in the grid
    * @param mousePos Mouse position relative to the window in the current frame
    */
    void updateHover(const sf::Vector2f& mousePos);


    /** Called when the window is clicked on, this function checks if a button has been clicked on
    * and is active, resulting in the number in said button and board position being changed to the number
    * the player is currently playing with
    * @param mousePos Mouse position relative to the window in the current frame
    * @param number The new number to replace the current number in the selected panel of the grid IF it is visible and active
    */
    void updateNumbers(const sf::Vector2f& mousePos, int number);


    //Activates the grid and every button on the grid
    void activate();

    //Deactivates the grid and every button on the grid
    void deactivate();

    /** Creates a new sudoku problem, removes numbers based on the difficulty,
    * and visualises this new problem on the grid
    */
    void appropriate();


    //Triggers the board shuffles every 5 turns and visualises this change to the sfml grid
    void krazyMode();

    //flag to identify whether the player is adding or removing numbers from the board
    static bool eraser_mode;

private:
    //The black frame of the sudoku grid
    sf::RectangleShape back;

    //The panels containing the numbers on the grid
    std::vector<Button> whiteBlocks;

    //A full, complete sudoku board
    Board finalBoard;

    /** Another sudoku board, but with some of its numbers removed.
    * 
    * This board is given to the player, visualised on the grid when a new game starts.
    * 
    * Its solution matches that of the final board
    */
    Board playersBoard = finalBoard;


    //Indicates how many turns the user has taken to complete the grid 
    int turns = 0;

    //Flag to ensure a shuffle occurs only once every 5 turns
    bool shuffled = false;
};

#endif