#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "themes.hpp"
#include "grid.hpp"
#include "main.hpp"
#include "button.hpp"

void changeNumber(Button& button, int& number) {    
    try {
        number = std::stoi(button.text.getString().toAnsiString());
        // use value
    } catch (const std::exception& e) {
        std::cerr << "Error converting text to int: " << e.what() << std::endl;
    }
}

int main() {
    Button numberChangers[9];

    sf::RenderWindow window(sf::VideoMode(600, 800), "Sudoku", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Create a graphical text to display
    sf::Font homeFont;
    if (!homeFont.loadFromFile("media/fonts/LoveDays.ttf")) {
        std::cerr << "Failed to load font.\n";
        return 1;
    }

    sf::Font gameFont;
    if (!gameFont.loadFromFile("media/fonts/Quicksand_Book.otf")) {
        std::cerr << "Failed to load font.\n";
        return 1;
    }

    /* Dynamic Background */
    //Define size of each coloured background panel
    float panelWidth = 300;
    float panelHeight = 800;

    sf:: RectangleShape panel1(sf::Vector2f(panelWidth, panelHeight));
    panel1.setPosition(0,0);

    sf:: RectangleShape panel2(sf::Vector2f(panelWidth, panelHeight));
    panel2.setPosition(panelWidth,0);

    float scrollSpeed = 100.f; //pixels per second
    sf::Vector2f offscreen = sf::Vector2f(200.f,800.f);
    sf::Clock clock;

    // Title Text
    sf::Text titleText;
    titleText.setFont(homeFont);
    titleText.setString("Sudoku!!");
    titleText.setCharacterSize(100); // in pixels, not points
    titleText.setPosition(sf::Vector2f(120.f, 50.f));

    // Home Menu Buttons Button
    Button easySwitch(200.0f, 100.0f, 200.0f, 250.0f, EASY_BUTTON, "Easy", homeFont);
    Button mediumSwitch(200.0f, 100.0f, 200.0f, 400.0f, MEDIUM_BUTTON, "Medium", homeFont);
    Button hardSwitch(200.0f, 100.0f, 200.0f, 550.0f, HARD_BUTTON, "Hard", homeFont);
 
    //Sudoku Grid
    Grid grid(sf::Vector2f(50.f, 150.f), 500.f, gameFont);
    grid.deactivate();

    //Number selection buttons
    int number = 1;
    for (int i = 0; i < 9; ++i) {
        Button button(500.f/9.f-5.f, 60.f, 55 + (i % 9) * 500.f/9.f, 680.f, REGULAR_BUTTON, std::to_string(i+1), gameFont);
        numberChangers[i] = button;
    }
    Button* chosenNumber = &numberChangers[0];
    chosenNumber->deactivate();

    //Load a music to play
    sf::Music music;
    if (!music.openFromFile("./media/music/Boo_Night_Fever.ogg")) {
        std::cerr << "Error finding music file";
        return -1;
    }
    //music.setLoop(true);
    //music.play();

    const Theme* currentTheme = &HOME_THEME;

    GameState stateFlag = STATE_HOME;

    //game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                
                if (easySwitch.frame.getGlobalBounds().contains(mousePos)) {
                    if (easySwitch.isActive()) {
                        stateFlag = STATE_EASY;
                        grid.appropriate(stateFlag);
                    }
                } else if (mediumSwitch.frame.getGlobalBounds().contains(mousePos)) {
                    if (mediumSwitch.isActive()) {
                        stateFlag = STATE_MEDIUM;
                        grid.appropriate(stateFlag);
                    }
                } else if (hardSwitch.frame.getGlobalBounds().contains(mousePos)) {
                    if (hardSwitch.isActive()) {
                        stateFlag = STATE_HARD;
                        grid.appropriate(stateFlag);
                    }
                }

                for (Button& button : numberChangers) {
                    if (button.frame.getGlobalBounds().contains(mousePos)) {
                        if (button.isActive()) {
                            changeNumber(button, number);
                            chosenNumber->activate();
                            chosenNumber = &button;
                            chosenNumber->deactivate();
                        }
                    }
                }

                grid.updateNumbers(mousePos, number);
                if (grid.check()) {
                    stateFlag = STATE_HOME;
                }
            }
        }

        switch (stateFlag) {
            case STATE_HOME:
                currentTheme = &HOME_THEME;
                break;
            case STATE_EASY:
                currentTheme = &EASY_THEME;
                break;
            case STATE_MEDIUM:
                currentTheme = &MEDIUM_THEME;
                break;
            case STATE_HARD:
                currentTheme = &HARD_THEME;
                break;
        }

        if (stateFlag != STATE_HOME) {
            //deactivating menu buttons
            easySwitch.activateMovement(offscreen, 600.f);
            mediumSwitch.activateMovement(offscreen, 600.f);
            hardSwitch.activateMovement(offscreen, 600.f);

            easySwitch.deactivate();
            mediumSwitch.deactivate();
            hardSwitch.deactivate();

            //activating number choice buttons
            for (Button& button : numberChangers) {
                if (&button != chosenNumber) {
                    button.activate();
                }
            }

            grid.activate();
        } else {
            //activating menu buttons
            easySwitch.activateMovement(easySwitch.getOriginalPos(), 400.f);
            mediumSwitch.activateMovement(mediumSwitch.getOriginalPos(), 400.f);
            hardSwitch.activateMovement(hardSwitch.getOriginalPos(), 400.f);

            easySwitch.activate();
            mediumSwitch.activate();
            hardSwitch.activate();

            //deactivating number choice buttons
            for (Button& button : numberChangers) {
                button.deactivate();
            }

            grid.deactivate();
        }

        //move panels to the right
        float deltaTime = clock.restart().asSeconds();
        panel1.move(scrollSpeed*deltaTime, 0);
        panel2.move(scrollSpeed*deltaTime, 0);

        //loop the panels when they go off-screen
        if (panel1.getPosition().x > 600) {
            panel1.setPosition(panel2.getPosition().x - 300, 0);
        }

        if (panel2.getPosition().x > 600) {
            panel2.setPosition(panel1.getPosition().x - 600, 0);
        }

        panel1.setFillColor(currentTheme -> bg1);
        panel2.setFillColor(currentTheme -> bg2);
        titleText.setFillColor(currentTheme -> text);

        if (stateFlag == STATE_HOME) {
            easySwitch.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
            mediumSwitch.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
            hardSwitch.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        } else {
            for (Button& button : numberChangers) {
                if (button.isActive()) {
                    button.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
                }
            }
            grid.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        }

        easySwitch.update(deltaTime);
        mediumSwitch.update(deltaTime);
        hardSwitch.update(deltaTime);
        for (int i = 0; i < 9; i++) {
            numberChangers[i].update(deltaTime);
        }

        //Draw everything necessary
        window.clear(currentTheme -> bgClear);
        window.draw(panel1);
        window.draw(panel2);
        if (stateFlag != STATE_HOME) {
            grid.display(window);
            for (Button& button : numberChangers) { //this loop
                button.display(window);
            }
        } else {
            window.draw(titleText);
        }
        easySwitch.display(window);
        mediumSwitch.display(window);
        hardSwitch.display(window);
        window.display();
    }
    //music.stop();
    return 0;
}
