#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include "sudoku.hpp"
#include "grid.hpp"
#include "button.hpp"

/** Takes the text contents of a button and sets the number the player is using to it
 *@param button The number changer the player clicks on
 *@param number The variable in main that's inputted into the grid when clicked on said grid
 */
void changeNumber(Button& button, int& number);

#endif