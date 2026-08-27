#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <memory>
#include "sudoku.hpp"
#include "grid.hpp"
#include "button.hpp"

/**
 * Main render window functions
 * @author Aura4G (Aria Noroozi)
*/

/// @brief Flag to identify whether or not the game is operating in full screen
extern bool isFullscreen;

/// @brief Game window size when not fullscreen
extern sf::Vector2f virtualSize;

/// @brief Game window dimensions
extern sf::View gameView;

/// @brief Vsync status of the open game
extern bool vsync;

/**
 * @brief Takes the text contents of a button and sets the number the player is using to it
 * @param button The number changer the player clicks on
 * @param number The variable in main that's inputted into the grid when clicked on said grid
*/
void changeNumber(Button& button, int& number);

void applyBackground(std::vector<sf::RectangleShape*>& panels, std::vector<sf::Sprite*>& bgs);

/**
 * @brief Creates an SFML window, either windowed or in exclusive fullscreen
 * @param fullscreen A flag to determine whether the window being created will be windowed
 *                   or fullscreen
 * @return The new window to replace the current render window
*/
std::unique_ptr<sf::RenderWindow> createWindow(bool fullscreen);

/**
 * @brief Scales window and its assets to new window size
 * @param target The render texture that's drawn by the game window
 * @param win The current game window
*/
void updateView(sf::RenderTexture& target, sf::RenderWindow& win);

///@brief The current state the game is in
extern GameState stateFlag;

#endif