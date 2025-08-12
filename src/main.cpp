#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cerrno>

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
        exit(errno);
    }
}

bool isFullscreen = true;
sf::Vector2f virtualSize(600.f, 800.f);
sf::View gameView(sf::FloatRect(0.f, 0.f, virtualSize.x, virtualSize.y));

std::unique_ptr<sf::RenderWindow> createWindow(bool fullscreen) {
    if (fullscreen) { //Renders a window in exclusive fullscreen if the fullscreen flag is checked
        sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
        return std::make_unique<sf::RenderWindow>(desktopMode, "Sudokrazy", sf::Style::None);
    } else { //Creates a smaller window using the virtual window otherwise
        return std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 800), "Sudokrazy", sf::Style::Titlebar | sf::Style::Close);
    }
}

void updateView(sf::RenderWindow& win) {
    //Get window size in pixels
    sf::Vector2u windowSize = win.getSize();

    //The aspect ratio of the actual window
    float windowRatio = static_cast<float>(windowSize.x) / windowSize.y;

    //The aspect ratio of the virtual window, the desired aspect ration
    float virtualRatio = virtualSize.x / virtualSize.y;

    //Describes the portion of the window where the view will be drawn. Starts out at 1 pixel
    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);

    if (windowRatio > virtualRatio) {
        //The window is wider than desired
        float scale = virtualRatio / windowRatio;
        viewport.left = (1.f - scale) / 2.f; //center horizontally
        viewport.width = scale;              //shrink width to match ratio
    } else {
        //The window is taller than desired
        float scale = windowRatio / virtualRatio;
        viewport.top = (1.f - scale) / 2.f; //center vertically
        viewport.height = scale;            //shrink height to match ratio
    }

    gameView.setViewport(viewport); // Apply normalized rectangle to view
    win.setView(gameView);          // Tell the window to use the adjusted view
}

sf::Color updateColour(sf::Color colour, sf::Color targetColour) {
    //Changes the colour scale to be closer to the target
    if (colour.r < targetColour.r) {
        colour.r++;
    } else if (colour.r > targetColour.r) {
        colour.r--;
    } //The colour's scale doesn't change if they match

    if (colour.g < targetColour.g) {
        colour.g++;
    } else if (colour.g > targetColour.g) {
        colour.g--;
    }

    if (colour.b < targetColour.b) {
        colour.b++;
    } else if (colour.b > targetColour.b) {
        colour.b--;
    }

    return colour;
}

int main() {
    /*Buttons that change the number the player puts on the board*/
    std::vector<Button> numberChangers;
    numberChangers.reserve(9);

    /* Window with fixed size and cannot fullscreen */
    auto window = createWindow(isFullscreen);
    updateView(*window);
    window->setPosition(sf::Vector2i(0,0));
    window->setFramerateLimit(60);

    /* Set window icon */
    sf::Image winIcon;
    if (!winIcon.loadFromFile("media/images/sudokrazy_logo.png")) {
        std::cerr << "Missing/Invalid image file\n";
        return errno;
    }
    window->setIcon(winIcon.getSize().x, winIcon.getSize().y, winIcon.getPixelsPtr());

    /* Load all media resources necessary */

    /* Fonts */

    ResourceManager::loadFont("homeFont", "media/fonts/LoveDays.ttf");
    ResourceManager::loadFont("gameFont", "media/fonts/Quicksand_Book.otf");

    /* Images */

    ResourceManager::loadTexture("placeholder", "media/images/placeholder.png");
    ResourceManager::loadTexture("title", "media/images/sudokrazy_title.png");
    ResourceManager::loadTexture("locked", "media/images/locked.png");
    ResourceManager::loadTexture("eraser", "media/images/eraser.png");


    /* Dynamic Background */
    //Define size of each coloured background panel
    float panelWidth = 300;
    float panelHeight = 800;

    sf:: RectangleShape panel1(sf::Vector2f(panelWidth, panelHeight));
    panel1.setPosition(0,0);

    sf:: RectangleShape panel2(sf::Vector2f(panelWidth, panelHeight));
    panel2.setPosition(panelWidth,0);

    sf:: RectangleShape panel3(sf::Vector2f(panelWidth, panelHeight));
    panel3.setPosition(-panelWidth,0);

    sf::Color clearTheme = {255,255,255};

    float scrollSpeed = 100.f; //pixels per second
    sf::Clock clock;

    //Title
    sf::Sprite title(ResourceManager::getTexture("title"));
    title.setScale(0.5f,0.5f);
    sf::FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width/2.0f, titleRect.top  + titleRect.height/2.0f);
    title.setPosition(sf::Vector2f(300,150));

    // Home Menu Buttons
    Button easySwitch(200.0f, 100.0f, 50.0f, 300.0f, EASY_BUTTON, "Easy", ResourceManager::getFont("gameFont"));
    Button mediumSwitch(200.0f, 100.0f, 350.0f, 300.0f, MEDIUM_BUTTON, "Medium", ResourceManager::getFont("gameFont"));
    Button hardSwitch(200.0f, 100.0f, 50.0f, 500.0f, HARD_BUTTON, "Hard", ResourceManager::getFont("gameFont"));
    Button krazySwitch(200.0f, 100.0f, 350.0f, 500.f, KRAZY_BUTTON, "KRAZY\nMODE!!", ResourceManager::getFont("gameFont"));
 
    //The visualised sudoku grid the player plays on
    Grid grid(sf::Vector2f(50.f, 150.f), 500.f);
    grid.deactivate();

    //Indicates the number the player is currently using
    int number = 1;
    //Iteration to construct the buttons used to switch numbers
    for (int i = 0; i < 9; ++i) {
        numberChangers.emplace_back(500.f/9.f-5.f, 60.f, 55 + (i % 9) * 500.f/9.f, 680.f, REGULAR_BUTTON, std::to_string(i+1), ResourceManager::getFont("gameFont"));
    }
    //Points to a number changing button to control its activity
    Button* chosenNumber = &numberChangers[0];
    chosenNumber->deactivate();

    //Back/Close button
    Button exit(25.f, 0.f, 35.f, 35.f, EXIT_BUTTON, "x", ResourceManager::getFont("homeFont"), ShapeType::Circle);

    //Eraser button
    Button eraser(37.5f, 0.f, 527.5f, 47.5f, REGULAR_BUTTON, "", ResourceManager::getFont("gameFont"), ShapeType::Circle);
    eraser.setTexture(ResourceManager::getTexture("eraser"), 5.f);

    //Music to play while game is operational
    sf::Music music;
    if (!music.openFromFile("./media/music/Boo_Night_Fever.ogg")) { //CHANGE MUSIC LATER
        std::cerr << "Error finding music file"; //error handling
        return errno;
    }
    //music.setLoop(true);
    //music.play();

    //Points to a color theme that matches the game state
    const Theme* currentTheme = &HOME_THEME;

    stateFlag = STATE_HOME;

    //game loop
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }

            //Conditions for each button when clicked on
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
                sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos, gameView);
                
                if (easySwitch.frame.getGlobalBounds().contains(worldPos)) { //starts an easy game
                    if (easySwitch.isActive()) {
                        stateFlag = STATE_EASY;
                        exit.setTheme(EASY_BUTTON);
                        eraser.setTheme(EASY_BUTTON);
                        grid.appropriate();
                    }
                } else if (mediumSwitch.frame.getGlobalBounds().contains(worldPos)) { //starts a medium game
                    if (mediumSwitch.isActive()) {
                        stateFlag = STATE_MEDIUM;
                        exit.setTheme(MEDIUM_BUTTON);
                        eraser.setTheme(MEDIUM_BUTTON);
                        grid.appropriate();
                    }
                } else if (hardSwitch.frame.getGlobalBounds().contains(worldPos)) { //starts a hard game
                    if (hardSwitch.isActive()) {
                        stateFlag = STATE_HARD;
                        exit.setTheme(HARD_BUTTON);
                        eraser.setTheme(HARD_BUTTON);
                        grid.appropriate();
                    }
                } else if (krazySwitch.frame.getGlobalBounds().contains(worldPos)) { //starts krazy mode
                    if (krazySwitch.isActive()) {
                        stateFlag = STATE_KRAZY;
                        exit.setTheme(KRAZY_BUTTON);
                        eraser.setTheme(KRAZY_BUTTON);
                        grid.appropriate();
                    }
                } else if (exit.circleFrame.getGlobalBounds().contains(worldPos) && exit.isActive()) {
                    if (stateFlag == STATE_HOME) { //clicking the exit button on the home screen closes the game
                        window->close();
                    } else { //clicking the exit button mid-game goes back to the home screen
                        stateFlag = STATE_HOME;
                        exit.setTheme(EXIT_BUTTON);
                        Grid::eraser_mode = false;
                    }
                } else if (eraser.circleFrame.getGlobalBounds().contains(worldPos) && eraser.isActive()) {
                    if (!Grid::eraser_mode) { //clicking the exit button on the home screen closes the game
                        Grid::eraser_mode = true;
                    } else { //clicking the exit button mid-game goes back to the home screen
                        Grid::eraser_mode = false;
                    }
                }

                for (Button& button : numberChangers) {
                    if (button.frame.getGlobalBounds().contains(worldPos)) { //switches the input number appropriately
                        if (button.isActive()) {
                            changeNumber(button, number);
                            chosenNumber->activate();
                            chosenNumber = &button;
                            chosenNumber->deactivate();
                        }
                    }
                }

                grid.updateNumbers(worldPos, number);
                if (grid.check()) { //checks if the grid has all its correct numbers
                    stateFlag = STATE_HOME;
                    exit.setTheme(EXIT_BUTTON);
                    Grid::eraser_mode = false;
                }
            }

            if (event.type == sf::Event::Resized) {
                updateView(*window); //updates the window if resized in this frame
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) { //MAKESHIFT window context switch
                isFullscreen = !isFullscreen;
                window->close();
                window = createWindow(isFullscreen);
                updateView(*window);
                window->setPosition(sf::Vector2i(0,0));
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
            easySwitch.activateMovement(sf::Vector2f(-210.f,easySwitch.getOriginalPos().y), 600.f);
            mediumSwitch.activateMovement(sf::Vector2f(610.f,mediumSwitch.getOriginalPos().y), 600.f);
            hardSwitch.activateMovement(sf::Vector2f(-210.f,hardSwitch.getOriginalPos().y), 600.f);
            krazySwitch.activateMovement(sf::Vector2f(610.f,krazySwitch.getOriginalPos().y), 600.f);

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
            eraser.activate();
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
            eraser.deactivate();
        }

        //move panels to the right
        float deltaTime = clock.restart().asSeconds();
        panel1.move(scrollSpeed*deltaTime, 0);
        panel2.move(scrollSpeed*deltaTime, 0);
        panel3.move(scrollSpeed*deltaTime, 0);

        //loop the panels when they go off-screen
        if (panel1.getPosition().x > 600) {
            panel1.setPosition(panel2.getPosition().x - 300, 0);
        }

        if (panel2.getPosition().x > 600) {
            panel2.setPosition(panel3.getPosition().x - 300, 0);
        }

        if (panel3.getPosition().x > 600) {
            panel3.setPosition(panel1.getPosition().x - 300, 0);
        }

        //set the colors to the theme
        panel1.setFillColor(updateColour(panel1.getFillColor(), currentTheme->bg1));
        panel2.setFillColor(updateColour(panel2.getFillColor(), currentTheme->bg2));
        panel3.setFillColor(updateColour(panel3.getFillColor(), currentTheme->bg3));

        if (stateFlag == STATE_HOME) { //menu buttons have hover visuals when on the home screen
            easySwitch.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
            mediumSwitch.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
            hardSwitch.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
            krazySwitch.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        } else { //ensures the buttons have hover visuals when playing the game and not on the home screen
            for (Button& button : numberChangers) {
                if (button.isActive()) {
                    button.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
                }
            }
            grid.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        }
        exit.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        if (!Grid::eraser_mode) {
            eraser.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        }

        //update the menu buttons each frame for gradual movement
        easySwitch.update(deltaTime);
        mediumSwitch.update(deltaTime);
        hardSwitch.update(deltaTime);
        krazySwitch.update(deltaTime);
        exit.update(deltaTime);
        eraser.update(deltaTime);
        for (int i = 0; i < 9; i++) {
            numberChangers.at(i).update(deltaTime);
        }

        //Draw everything necessary
        clearTheme = updateColour(clearTheme, currentTheme->bgClear);
        window->clear(clearTheme);
        window->setView(gameView);
        window->draw(panel1);
        window->draw(panel2);
        window->draw(panel3);
        if (stateFlag != STATE_HOME) {
            grid.display(*window);
            for (Button& button : numberChangers) { //this loop
                button.display(*window);
            }
            eraser.display(*window);
        } else {
            window->draw(title);
        }
        easySwitch.display(*window);
        mediumSwitch.display(*window);
        hardSwitch.display(*window);
        krazySwitch.display(*window);
        exit.display(*window);
        window->display();
    }
    //music.stop();
    return 0;
}
