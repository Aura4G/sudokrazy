#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "themes.hpp"
#include "grid.hpp"
#include "main.hpp"
#include "button.hpp"
#include "resource_manager.hpp"

GameState stateFlag = STATE_HOME;

void changeNumber(Button& button, int& number) {    
    try {
        number = std::stoi(button.text.getString().toAnsiString());
        // use value
    } catch (const std::exception& e) { //error handling (this shouldn't occur at all given the buttons used)
        std::cerr << "Error converting text to int: " << e.what() << std::endl;
    }
}

int main() {
    /*Buttons that change the number the player puts on the board*/
    Button numberChangers[9];

    /* Window with fixed size and cannot fullscreen */
    sf::RenderWindow window(sf::VideoMode(600, 800), "Sudoku", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    /*Load all media resources necessary*/
    ResourceManager::loadFont("homeFont", "media/fonts/LoveDays.ttf");
    ResourceManager::loadFont("gameFont", "media/fonts/Quicksand_Book.otf");

    /* Dynamic Background */
    //Define size of each coloured background panel
    float panelWidth = 300;
    float panelHeight = 800;

    sf:: RectangleShape panel1(sf::Vector2f(panelWidth, panelHeight));
    panel1.setPosition(0,0);

    sf:: RectangleShape panel2(sf::Vector2f(panelWidth, panelHeight));
    panel2.setPosition(panelWidth,0);

    float scrollSpeed = 100.f; //pixels per second
    sf::Clock clock;

    // Title Text
    sf::Text titleText;
    titleText.setFont(ResourceManager::getFont("homeFont"));
    titleText.setString("SudoKrazy!!");
    titleText.setCharacterSize(100); // in pixels, not points
    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    titleText.setPosition(sf::Vector2f(300,150));

    // Home Menu Buttons
    Button easySwitch(200.0f, 100.0f, 50.0f, 300.0f, EASY_BUTTON, "Easy", ResourceManager::getFont("homeFont"));
    Button mediumSwitch(200.0f, 100.0f, 350.0f, 300.0f, MEDIUM_BUTTON, "Medium", ResourceManager::getFont("homeFont"));
    Button hardSwitch(200.0f, 100.0f, 50.0f, 500.0f, HARD_BUTTON, "Hard", ResourceManager::getFont("homeFont"));
    Button krazySwitch(200.0f, 100.0f, 350.0f, 500.f, KRAZY_BUTTON, "KRAZY\nMODE!!", ResourceManager::getFont("homeFont"));
 
    //The visualised sudoku grid the player plays on
    Grid grid(sf::Vector2f(50.f, 150.f), 500.f, ResourceManager::getFont("gameFont"));
    grid.deactivate();

    //Indicates the number the player is currently using
    int number = 1;
    //Iteration to construct the buttons used to switch numbers
    for (int i = 0; i < 9; ++i) {
        Button button(500.f/9.f-5.f, 60.f, 55 + (i % 9) * 500.f/9.f, 680.f, REGULAR_BUTTON, std::to_string(i+1), ResourceManager::getFont("gameFont"));
        numberChangers[i] = button;
    }
    //Points to a number changing button to control its activity
    Button* chosenNumber = &numberChangers[0];
    chosenNumber->deactivate();

    //Back/Close button
    Button exit(25.f, 0.f, 35.f, 35.f, EXIT_BUTTON, "x", ResourceManager::getFont("homeFont"), ShapeType::Circle);

    //Music to play while game is operational
    sf::Music music;
    if (!music.openFromFile("./media/music/Boo_Night_Fever.ogg")) { //CHANGE MUSIC LATER
        std::cerr << "Error finding music file"; //error handling
        return -1;
    }
    //music.setLoop(true);
    //music.play();

    //Points to a color theme that matches the game state
    const Theme* currentTheme = &HOME_THEME;

    stateFlag = STATE_HOME;

    //game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //Conditions for each button when clicked on
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                
                if (easySwitch.frame.getGlobalBounds().contains(mousePos)) { //starts an easy game
                    if (easySwitch.isActive()) {
                        stateFlag = STATE_EASY;
                        exit.setTheme(EASY_BUTTON);
                        grid.appropriate();
                    }
                } else if (mediumSwitch.frame.getGlobalBounds().contains(mousePos)) { //starts a medium game
                    if (mediumSwitch.isActive()) {
                        stateFlag = STATE_MEDIUM;
                        exit.setTheme(MEDIUM_BUTTON);
                        grid.appropriate();
                    }
                } else if (hardSwitch.frame.getGlobalBounds().contains(mousePos)) { //starts a hard game
                    if (hardSwitch.isActive()) {
                        stateFlag = STATE_HARD;
                        exit.setTheme(HARD_BUTTON);
                        grid.appropriate();
                    }
                } else if (krazySwitch.frame.getGlobalBounds().contains(mousePos)) { //starts krazy mode
                    if (krazySwitch.isActive()) {
                        stateFlag = STATE_KRAZY;
                        exit.setTheme(KRAZY_BUTTON);
                        grid.appropriate();
                    }
                } else if (exit.circleFrame.getGlobalBounds().contains(mousePos) && exit.isActive()) {
                    if (stateFlag == STATE_HOME) { //clicking the exit button on the home screen closes the game
                        window.close();
                    } else { //clicking the exit button mid-game goes back to the home screen
                        stateFlag = STATE_HOME;
                        exit.setTheme(EXIT_BUTTON);
                    }
                }

                for (Button& button : numberChangers) {
                    if (button.frame.getGlobalBounds().contains(mousePos)) { //switches the input number appropriately
                        if (button.isActive()) {
                            changeNumber(button, number);
                            chosenNumber->activate();
                            chosenNumber = &button;
                            chosenNumber->deactivate();
                        }
                    }
                }

                grid.updateNumbers(mousePos, number);
                if (grid.check()) { //checks if the grid has all its correct numbers
                    stateFlag = STATE_HOME;
                    exit.setTheme(EXIT_BUTTON);
                }
            }
        }

        switch (stateFlag) { //switch graphical themes depending on the game state
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
            case STATE_KRAZY:
                currentTheme = &KRAZY_THEME;
                break;
        }

        if (stateFlag != STATE_HOME) {
            //deactivating menu buttons
            easySwitch.activateMovement(sf::Vector2f(-200.f,easySwitch.getOriginalPos().y), 600.f);
            mediumSwitch.activateMovement(sf::Vector2f(600.f,mediumSwitch.getOriginalPos().y), 600.f);
            hardSwitch.activateMovement(sf::Vector2f(-200.f,hardSwitch.getOriginalPos().y), 600.f);
            krazySwitch.activateMovement(sf::Vector2f(600.f,krazySwitch.getOriginalPos().y), 600.f);

            easySwitch.deactivate();
            mediumSwitch.deactivate();
            hardSwitch.deactivate();
            krazySwitch.deactivate();

            exit.setText("<-");

            //activating number choice buttons
            for (Button& button : numberChangers) {
                if (&button != chosenNumber) {
                    button.activate();
                }
            }

            //activates the grid and its buttons
            grid.activate();
        } else {
            //activating menu buttons
            easySwitch.activateMovement(easySwitch.getOriginalPos(), 400.f);
            mediumSwitch.activateMovement(mediumSwitch.getOriginalPos(), 400.f);
            hardSwitch.activateMovement(hardSwitch.getOriginalPos(), 400.f);
            krazySwitch.activateMovement(krazySwitch.getOriginalPos(), 400.f);

            easySwitch.activate();
            mediumSwitch.activate();
            hardSwitch.activate();
            krazySwitch.activate();

            exit.setText("x");

            //deactivating number choice buttons
            for (Button& button : numberChangers) {
                button.deactivate();
            }

            //ensures no inputs on the invisible grid can be made when outside gameplay mode
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

        //set the colors to the theme
        panel1.setFillColor(currentTheme -> bg1);
        panel2.setFillColor(currentTheme -> bg2);
        titleText.setFillColor(currentTheme -> text);

        if (stateFlag == STATE_HOME) { //menu buttons have hover visuals when on the home screen
            easySwitch.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
            mediumSwitch.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
            hardSwitch.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
            krazySwitch.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        } else { //ensures the buttons have hover visuals when playing the game and not on the home screen
            for (Button& button : numberChangers) {
                if (button.isActive()) {
                    button.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
                }
            }
            grid.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        }
        exit.updateHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

        //update the menu buttons each frame for gradual movement
        easySwitch.update(deltaTime);
        mediumSwitch.update(deltaTime);
        hardSwitch.update(deltaTime);
        krazySwitch.update(deltaTime);
        exit.update(deltaTime);
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
        krazySwitch.display(window);
        exit.display(window);
        window.display();
    }
    //music.stop();
    return 0;
}
