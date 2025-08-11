#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <memory>
#include "sudoku.hpp"
#include "grid.hpp"
#include "button.hpp"

//Flag to identify whether or not the game is operating in full screen
extern bool isFullscreen;

//Game window size when not fullscreen
extern sf::Vector2f virtualSize;

//Game window dimensions
extern sf::View gameView;

/** Takes the text contents of a button and sets the number the player is using to it
 *@param button The number changer the player clicks on
 *@param number The variable in main that's inputted into the grid when clicked on said grid
 */
void changeNumber(Button& button, int& number);

std::unique_ptr<sf::RenderWindow> createWindow(bool fullscreen);

void updateView(sf::RenderWindow& win);

//The current state the game is in
extern GameState stateFlag;

#endif