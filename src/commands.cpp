#include <iostream>
#include <SFML/Graphics.hpp>

#include "sudoku.hpp"
#include "grid.hpp"
#include "button.hpp"
#include "commands.hpp"

void changeNumber(Button& button, int& number) {    
    try {
        number = std::stoi(button.text.getString().toAnsiString());
        // use value
    } catch (const std::exception& e) {
        std::cerr << "Error converting text to int: " << e.what() << std::endl;
    }
}