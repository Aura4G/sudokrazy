#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cerrno>
#include <cmath>

#include "themes.hpp"
#include "grid.hpp"
#include "main.hpp"
#include "button.hpp"
#include "resource_manager.hpp"
#include "slider.hpp"
#include "record.hpp"
#include "shop.hpp"
#include "shop_manager.hpp"

GameState stateFlag = STATE_HOME;

bool vsync = true;

void changeNumber(Button& button, int& number) {    
    try {
        number = std::stoi(button.text.getString().toAnsiString());
        // use value
    } catch (const std::exception& e) { // Error handling (this shouldn't occur at all given the buttons used)
        std::cerr << "Error converting text to int: " << e.what() << std::endl;
        exit(errno);
    }
}

void applyBackground(std::vector<sf::RectangleShape*>& panels, std::vector<sf::Sprite*>& bgs) {
    std::string key;

    for (Item item : ShopManager::getEquips()) {
        if (item.getType() == ItemType::Background) {
            key = item.getEquipKey();
        }
    }

    if (key == "") return;

    for (int i = 0; i < 3; i++) {
        bgs.at(i)->setTexture(ResourceManager::getTexture(key + std::to_string(i+1)));
        bgs.at(i)->setTextureRect(sf::IntRect(0, 0, ResourceManager::getTexture(key + std::to_string(i+1)).getSize().x, ResourceManager::getTexture(key + std::to_string(i+1)).getSize().y));

        // Get bounds of the sprite
        sf::FloatRect subjectBounds = bgs.at(i)->getLocalBounds();
        sf::FloatRect targetRect = panels.at(i)->getLocalBounds();

        // Get available width and height from button frame
        float availableWidth = targetRect.width;
        float availableHeight = targetRect.height;

        // Calculate a scale factor from our available widths and heights
        float scaleX = availableWidth / subjectBounds.width;
        float scaleY = availableHeight / subjectBounds.height;
        float scale = std::min(scaleX, scaleY); //choose the minimum between the two scales to ensure it fits in the bounds

        // Apply scale
        bgs.at(i)->setScale(scale,scale);

        sf::Vector2f pos = panels.at(i)->getPosition();
        sf::Vector2f size = panels.at(i)->getSize();

        // Center the origin of the scaled sprite
        subjectBounds = bgs.at(i)->getLocalBounds();
        bgs.at(i)->setOrigin(subjectBounds.left + subjectBounds.width / 2.0f, subjectBounds.top + subjectBounds.height / 2.0f);
        bgs.at(i)->setPosition(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
    }
}

bool isFullscreen = false;
sf::Vector2f virtualSize(600.f, 800.f);
sf::View gameView(sf::FloatRect(0.f, 0.f, virtualSize.x, virtualSize.y));

std::unique_ptr<sf::RenderWindow> createWindow(bool fullscreen) {
    if (fullscreen) { // Renders a window in exclusive fullscreen if the fullscreen flag is checked
        sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
        return std::make_unique<sf::RenderWindow>(desktopMode, "Sudokrazy", sf::Style::None);
    } else { // Creates a smaller window using the virtual window otherwise
        return std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 800), "Sudokrazy", sf::Style::Titlebar | sf::Style::Close);
    }
}

void updateView(sf::RenderTexture& target, sf::RenderWindow& win) {
    // Get window size in pixels
    sf::Vector2u windowSize = win.getSize();

    // The aspect ratio of the actual window
    float windowRatio = static_cast<float>(windowSize.x) / windowSize.y;

    // The aspect ratio of the virtual window, the desired aspect ration
    float virtualRatio = virtualSize.x / virtualSize.y;

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    // Describes the portion of the window where the view will be drawn. Starts out at 1 pixel
    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);

    if (windowRatio > virtualRatio) {
        // The window is wider than desired
        float scale = virtualRatio / windowRatio;
        viewport.left = (1.f - scale) / 2.f; // Center horizontally
        viewport.width = scale;              // Shrink width to match ratio
    } else {
        // The window is taller than desired
        float scale = windowRatio / virtualRatio;
        viewport.top = (1.f - scale) / 2.f; // Center vertically
        viewport.height = scale;            // Shrink height to match ratio
    }

    gameView.setViewport(viewport); // Apply normalized rectangle to view
    
    target.setView(gameView); // Apply view to the render texture
    
    win.setView(win.getDefaultView()); // Tell the window to use the adjusted view
}

int main() {
    /* Load .dat files */

    // Load all the completed games into the save manager's records vector
    SaveManager::loadRecords("records.dat");

    // Load all settings configurations previously left in the last settings menu session
    SaveManager::loadSettings("settings.dat");

    // Enable V-Sync according to the settings configuration
    vsync = SaveManager::getVSync();

    // Enable Fullscreen according the settings configuration
    isFullscreen = SaveManager::getFullscreen();


    /* Buttons that change the number the player puts on the board */
    std::vector<Button> numberChangers;
    numberChangers.reserve(9);

    /* Window with fixed size and cannot fullscreen */
    auto window = createWindow(isFullscreen);

    // Create a render texture to draw everything to before post-processing and displaying
    sf::RenderTexture renderTexture;
    renderTexture.create(window->getSize().x, window->getSize().y);

    updateView(renderTexture, *window);

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    window->setPosition(sf::Vector2i(static_cast<int>(desktopMode.width/2 - window->getSize().x/2),static_cast<int>(desktopMode.height/2 - window->getSize().y/2)));

    window->setVerticalSyncEnabled(vsync);

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
    ResourceManager::loadTexture("eraser", "media/images/eraser.png");
    ResourceManager::loadTexture("settings", "media/images/settings.png");
    ResourceManager::loadTexture("settings title", "media/images/settings_title.png");
    ResourceManager::loadTexture("lock", "media/images/lock.png");

    /* Backgrounds */
    // Test
    ResourceManager::loadTexture("test1", "media/images/backgrounds/test1.png");
    ResourceManager::loadTexture("test2", "media/images/backgrounds/test2.png");
    ResourceManager::loadTexture("test3", "media/images/backgrounds/test3.png");

    // Space
    ResourceManager::loadTexture("space1", "media/images/backgrounds/space1.png");
    ResourceManager::loadTexture("space2", "media/images/backgrounds/space2.png");
    ResourceManager::loadTexture("space3", "media/images/backgrounds/space3.png");

    /* Audio */

    ResourceManager::loadAudio("main theme", "media/music/sudokrazy_main.ogg");

    /* Shaders */

    ResourceManager::loadShader("shader", "media/shaders/shader.frag");

    sf::Shader& shader = ResourceManager::getShader("shader");
    float currentBrightness = 1.0f; // Normal brightness
    float contrast = 1.0f; // Normal contrast


    /* Dynamic Background */
    // Define size of each coloured background panel
    float panelWidth = 300;
    float panelHeight = 800;

    sf:: RectangleShape panel1(sf::Vector2f(panelWidth, panelHeight));
    panel1.setPosition(0,0);
    sf::Sprite bg1(ResourceManager::getTexture("placeholder"), sf::IntRect(0, 0, ResourceManager::getTexture("placeholder").getSize().x, ResourceManager::getTexture("placeholder").getSize().y));

    sf:: RectangleShape panel2(sf::Vector2f(panelWidth, panelHeight));
    panel2.setPosition(panelWidth,0);
    sf::Sprite bg2(ResourceManager::getTexture("placeholder"), sf::IntRect(0, 0, ResourceManager::getTexture("placeholder").getSize().x, ResourceManager::getTexture("placeholder").getSize().y));

    sf:: RectangleShape panel3(sf::Vector2f(panelWidth, panelHeight));
    panel3.setPosition(-panelWidth,0);
    sf::Sprite bg3(ResourceManager::getTexture("placeholder"), sf::IntRect(0, 0, ResourceManager::getTexture("placeholder").getSize().x, ResourceManager::getTexture("placeholder").getSize().y));

    sf::Color clearTheme = {255,255,255};

    float scrollSpeed = 100.f; // Pixels per second
    sf::Clock clock;

    // Title
    sf::Sprite title(ResourceManager::getTexture("title"));
    title.setScale(0.5f,0.5f);
    sf::FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width/2.0f, titleRect.top  + titleRect.height/2.0f);
    title.setPosition(sf::Vector2f(300,150));

    // Home Menu Buttons

    Button easySwitch(200.0f, 100.0f, 50.0f, 300.0f, EASY_BUTTON, "Easy", "gameFont");

    Button mediumSwitch(200.0f, 100.0f, 350.0f, 300.0f, MEDIUM_BUTTON, "Medium", "gameFont");
    mediumSwitch.setTexture(ResourceManager::getTexture("lock"), 5.f);
    mediumSwitch.deactivate();

    Button hardSwitch(200.0f, 100.0f, 50.0f, 500.0f, HARD_BUTTON, "Hard", "gameFont");
    hardSwitch.setTexture(ResourceManager::getTexture("lock"), 5.f);
    hardSwitch.deactivate();

    Button krazySwitch(200.0f, 100.0f, 350.0f, 500.f, KRAZY_BUTTON, "KRAZY\nMODE!!", "gameFont");
    krazySwitch.setTexture(ResourceManager::getTexture("lock"), 5.f);
    krazySwitch.deactivate();
 
    // The visualised sudoku grid the player plays on
    Grid grid(sf::Vector2f(50.f, 150.f), 500.f);
    grid.deactivate();

    // Indicates the number the player is currently using
    int number = 1;
    // Iteration to construct the buttons used to switch numbers
    for (int i = 0; i < 9; ++i) {
        numberChangers.emplace_back(500.f/9.f-5.f, 60.f, 55 + (i % 9) * 500.f/9.f, 680.f, REGULAR_BUTTON, std::to_string(i+1), "gameFont");
    }
    // Points to a number changing button to control its activity
    Button* chosenNumber = &numberChangers[0];
    chosenNumber->deactivate();

    // Back/Close button
    Button exit(25.f, 0.f, 35.f, 35.f, EXIT_BUTTON, "x", "homeFont", ShapeType::Circle);

    // Eraser button
    Button eraser(37.5f, 0.f, 470.f, 47.5f, REGULAR_BUTTON, "", "gameFont", ShapeType::Circle);
    eraser.setTexture(ResourceManager::getTexture("eraser"), 5.f);

    // Hint button
    Button hint(37.5f, 0.f, 390.f, 47.5f, REGULAR_BUTTON, "0", "gameFont", ShapeType::Circle);


    /* SETTINGS MENU */

    Button settingsToggle(37.5f, 0.f, 550.f, 47.5f, REGULAR_BUTTON, "", "gameFont", ShapeType::Circle);
    settingsToggle.setTexture(ResourceManager::getTexture("settings"), 10.f);

    // The settings menu art
    sf::Sprite settingsTitle(ResourceManager::getTexture("settings title"));
    titleRect = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin(titleRect.left + titleRect.width/2.0f, titleRect.top  + titleRect.height/2.0f);
    settingsTitle.setPosition(sf::Vector2f(300,100));

    // Button to toggle V-Sync on/off
    Button vsyncToggle(150.f, 75.f, 100.f, 220.f, (vsync ? EASY_BUTTON : HARD_BUTTON), (vsync ? "V-Sync On" : "V-Sync Off"), "gameFont");

    // Button to toggle fullscreen on/off
    Button fullscreenToggle(150.f, 75.f, 350.f, 220.f, (isFullscreen ? EASY_BUTTON : HARD_BUTTON), (isFullscreen ? "Fullscreen" : "Windowed"), "gameFont");


    /* Volume */

    sf::Text volumeText;
    volumeText.setString("Volume:");
    volumeText.setFont(ResourceManager::getFont("gameFont"));
    volumeText.setCharacterSize(16);
    volumeText.setFillColor(sf::Color::Black);
    volumeText.setPosition(sf::Vector2f(100.f, 320.f));

    // Changes the Volume
    // Retrieves the saved volume setting for the initial setting
    Slider volumeSlider(400.f, sf::Vector2f(100.f, 360.f), SaveManager::getVolume(), sf::Color::Black);


    /* Brightness */

    sf::Text brightnessText;
    brightnessText.setString("Brightness:");
    brightnessText.setFont(ResourceManager::getFont("gameFont"));
    brightnessText.setCharacterSize(16);
    brightnessText.setFillColor(sf::Color::Black);
    brightnessText.setPosition(sf::Vector2f(100.f, 400.f));

    // Changes the Brightness
    // Retrieves the saved brightness setting for the initial setting
    Slider brightnessSlider(400.f, sf::Vector2f(100.f, 440.f), SaveManager::getBrightness(), sf::Color::Black);


    /* Contrast */

    sf::Text contrastText;
    contrastText.setString("Contrast:");
    contrastText.setFont(ResourceManager::getFont("gameFont"));
    contrastText.setCharacterSize(16);
    contrastText.setFillColor(sf::Color::Black);
    contrastText.setPosition(sf::Vector2f(100.f, 480.f));

    // Changes the Contrast
    // Retrieves the saved contrast setting for the initial setting
    Slider contrastSlider(400.f, sf::Vector2f(100.f, 520.f), SaveManager::getContrast(), sf::Color::Black);


    /* Bg Scroller */

    sf::Text scrollerText;
    scrollerText.setString("Background scroll speed:");
    scrollerText.setFont(ResourceManager::getFont("gameFont"));
    scrollerText.setCharacterSize(16);
    scrollerText.setFillColor(sf::Color::Black);
    scrollerText.setPosition(sf::Vector2f(100.f, 560.f));

    // Changes the background scroll speed
    // Retrieves the saved speed setting for the initial setting
    Slider scrollerSlider(400.f, sf::Vector2f(100.f, 600.f), SaveManager::getBSpeed(), sf::Color::Black);


    /* SHOP */
    Button shopToggle(37.5f, 0.f, 470.f, 47.5f, REGULAR_BUTTON, "SHOP", "homeFont", ShapeType::Circle);

    Shop::addItem(Item(1, "Hint", "lock"));
    Shop::addItem(Item(2, "Outdoor Theme", "settings", "test", ItemType::Background));
    Shop::addItem(Item(5, "Space Theme", "settings", "space", ItemType::Background));

    Shelf shopShelf(3, 2, sf::Vector2f(50.f,200.f), sf::Vector2f(150.f, 150.f), sf::Vector2f(25.f, 40.f));

    // Load all purchases and equips from the shop AFTER the shop items have been added
    ShopManager::loadShop("shop.dat");

    // Put shop's content onto button grid "shelf"
    shopShelf.pullShop();

    // The number of hints read from the save file must appear initially
    hint.setText(std::to_string(ShopManager::getHints()));

    /* EQUIPS */

    // Music to play while game is operational
    sf::Music& music = ResourceManager::getAudio("main theme");
    music.setLoop(true);
    music.play();


    // Points to a color theme that matches the game state
    const Theme* currentTheme = &HOME_THEME;

    stateFlag = STATE_HOME;
    GameState prevState = STATE_HOME;

    sf::Clock timer;
    int score = 0;
    int cumulativeScore = SaveManager::getTotalScore();
    float timeGap = 0;
    sf::Time previousHit = timer.getElapsedTime();

    float timeOut = 0;
    float totalTimeOut = 0;

    sf::Text scoreText;
    scoreText.setString("");
    scoreText.setFont(ResourceManager::getFont("gameFont"));
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(currentTheme->text);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(1);

    sf::FloatRect textRect = scoreText.getLocalBounds();
    scoreText.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
    scoreText.setPosition(sf::Vector2f(300.f, 100.f));

    sf::Text culScoreText;
    culScoreText.setString("Total Score: 0");
    culScoreText.setFont(ResourceManager::getFont("homeFont"));
    culScoreText.setCharacterSize(42);
    culScoreText.setFillColor(HOME_THEME.text);
    culScoreText.setOutlineColor(sf::Color::Black);
    culScoreText.setOutlineThickness(1);

    textRect = culScoreText.getLocalBounds();
    culScoreText.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
    culScoreText.setPosition(sf::Vector2f(300.f, 650.f));

    sf::Text pointsText;
    pointsText.setString("Kuukies: 0");
    pointsText.setFont(ResourceManager::getFont("homeFont"));
    pointsText.setCharacterSize(42);
    pointsText.setFillColor(HOME_THEME.text);
    pointsText.setOutlineColor(sf::Color::Black);
    pointsText.setOutlineThickness(1);

    textRect = pointsText.getLocalBounds();
    pointsText.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
    pointsText.setPosition(sf::Vector2f(300.f, 700.f));

    sf::Text timerText;
    timerText.setString("");
    timerText.setFont(ResourceManager::getFont("gameFont"));
    timerText.setCharacterSize(30);
    timerText.setFillColor(currentTheme->text);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(1);

    textRect = timerText.getLocalBounds();
    timerText.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
    timerText.setPosition(sf::Vector2f(300.f, 50.f));

    std::vector<sf::RectangleShape*> panels = {&panel1, &panel2, &panel3};
    std::vector<sf::Sprite*> bgs = {&bg1, &bg2, &bg3};
    applyBackground(panels, bgs);

    // Game loop
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }

            // Conditions for each button when clicked on
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
                sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos, gameView);
                
                if (easySwitch.frame.getGlobalBounds().contains(worldPos)) { // Starts an easy game
                    if (easySwitch.isActive()) {
                        timer.restart();
                        previousHit = timer.getElapsedTime();
                        score = 0;
                        totalTimeOut = 0;
                        timeOut = 0;
                        prevState = stateFlag;
                        stateFlag = STATE_EASY;
                        grid.appropriate();
                    }
                } else if (mediumSwitch.frame.getGlobalBounds().contains(worldPos)) { // Starts a medium game
                    if (mediumSwitch.isActive()) {
                        timer.restart();
                        previousHit = timer.getElapsedTime();
                        score = 0;
                        totalTimeOut = 0;
                        timeOut = 0;
                        prevState = stateFlag;
                        stateFlag = STATE_MEDIUM;
                        grid.appropriate();
                    }
                } else if (hardSwitch.frame.getGlobalBounds().contains(worldPos)) { // Starts a hard game
                    if (hardSwitch.isActive()) {
                        timer.restart();
                        previousHit = timer.getElapsedTime();
                        score = 0;
                        totalTimeOut = 0;
                        timeOut = 0;
                        prevState = stateFlag;
                        stateFlag = STATE_HARD;
                        grid.appropriate();
                    }
                } else if (krazySwitch.frame.getGlobalBounds().contains(worldPos)) { // Starts krazy mode
                    if (krazySwitch.isActive()) {
                        timer.restart();
                        previousHit = timer.getElapsedTime();
                        score = 0;
                        totalTimeOut = 0;
                        timeOut = 0;
                        prevState = stateFlag;
                        stateFlag = STATE_KRAZY;
                        grid.appropriate();
                    }
                } else if (exit.circleFrame.getGlobalBounds().contains(worldPos) && exit.isActive()) {
                    if (stateFlag == STATE_HOME) { // Clicking the exit button on the home screen closes the game
                        window->close();
                    } else { // Clicking the exit button mid-game goes back to the home screen
                        Grid::eraser_mode = false;
                        if (!settingsToggle.isActive()) { // Condition when leaving settings menu
                            stateFlag = prevState;
                            prevState = STATE_SETTINGS;
                            SaveManager::saveSettings("settings.dat"); // Saves settings when leaving
                        } else { // Condition for leaving an open game
                            prevState = stateFlag;
                            stateFlag = STATE_HOME;
                        }

                        if (stateFlag >= STATE_EASY && stateFlag <= STATE_KRAZY) {
                            // Calculates the total time spent in the settings menu outside of an active game
                            // So that it can deduct that time-out from the in-game timer

                            timeOut = timer.getElapsedTime().asSeconds() - timeOut;
                            totalTimeOut += timeOut;
                        }
                    }
                } else if (eraser.circleFrame.getGlobalBounds().contains(worldPos) && eraser.isActive()) {
                    if (!Grid::eraser_mode) { // Clicking the exit button on the home screen closes the game
                        Grid::eraser_mode = true;
                    } else { // Clicking the exit button mid-game goes back to the home screen
                        Grid::eraser_mode = false;
                    }
                } else if (hint.circleFrame.getGlobalBounds().contains(worldPos) && hint.isActive() && ShopManager::getHints() > 0) {
                    ShopManager::alterHints(-1);
                    ShopManager::saveInfo("shop.dat");
                    grid.useHint();
                } else if (settingsToggle.circleFrame.getGlobalBounds().contains(worldPos)) {
                    if (settingsToggle.isActive()) {
                        if (stateFlag >= STATE_EASY && stateFlag <= STATE_KRAZY) {
                            timeOut = timer.getElapsedTime().asSeconds();
                        }

                        prevState = stateFlag;
                        stateFlag = STATE_SETTINGS;
                    }
                } else if (vsyncToggle.frame.getGlobalBounds().contains(worldPos)) {
                    if (vsyncToggle.isActive()) {
                        if (vsync) {
                            vsync = false;
                            SaveManager::setVSync(vsync); // Saves vsync setting
                            vsyncToggle.setTheme(HARD_BUTTON);
                            vsyncToggle.setText("V-Sync Off");
                        } else {
                            vsync = true;
                            SaveManager::setVSync(vsync); // Saves vsync setting
                            vsyncToggle.setTheme(EASY_BUTTON);
                            vsyncToggle.setText("V-Sync On");
                        }
                        window->setVerticalSyncEnabled(vsync);
                        window->setFramerateLimit(vsync ? 0 : 60); // Optional cap when vsync off
                        clock.restart();
                    }
                } else if (fullscreenToggle.frame.getGlobalBounds().contains(worldPos)) {
                    if (fullscreenToggle.isActive()) {
                        isFullscreen = !isFullscreen;
                        SaveManager::setFullscreen(isFullscreen); // Saves window setting
                        window->close();

                        window = createWindow(isFullscreen);

                        renderTexture.create(window->getSize().x, window->getSize().y);

                        updateView(renderTexture, *window);
                        window->setPosition(sf::Vector2i(static_cast<int>(desktopMode.width/2 - window->getSize().x/2),static_cast<int>(desktopMode.height/2 - window->getSize().y/2)));

                        // Reapply timing policy

                        window->setVerticalSyncEnabled(vsync);

                        // Reapply icon since it's a new window
                        window->setIcon(winIcon.getSize().x, winIcon.getSize().y, winIcon.getPixelsPtr());

                        clock.restart(); // Ensure next deltaTime is small/clean

                        fullscreenToggle.setTheme(isFullscreen ? EASY_BUTTON : HARD_BUTTON);
                        fullscreenToggle.setText(isFullscreen ? "Fullscreen" : "Windowed");
                    }
                } else if (shopToggle.circleFrame.getGlobalBounds().contains(worldPos)) {
                    if (shopToggle.isActive()) {
                        prevState = stateFlag;
                        stateFlag = STATE_SHOP;
                    }
                }

                for (Button& button : numberChangers) {
                    if (button.frame.getGlobalBounds().contains(worldPos)) { // Switches the input number appropriately
                        if (button.isActive()) {
                            changeNumber(button, number);
                            grid.changeIndicator(number);
                            chosenNumber->activate();
                            chosenNumber = &button;
                            chosenNumber->deactivate();
                        }
                    }
                }

                if (grid.updateNumbers(worldPos, number)) { // Score is added if the grid is updated with a correct number
                    timeGap = timer.getElapsedTime().asSeconds() - previousHit.asSeconds();
                    score += (timeGap >= 60.f ? 5 * stateFlag : static_cast<int>(stateFlag * (50 - 0.75 * (timeGap - totalTimeOut))));
                    previousHit = timer.getElapsedTime();
                }

                if (shopShelf.updateShelf(worldPos)) {
                    applyBackground(panels, bgs);
                }

                hint.setText(std::to_string(ShopManager::getHints()));
                
                if (grid.check()) { // Checks if the grid has all its correct numbers
                    Record game(score, stateFlag, sf::seconds(timer.getElapsedTime().asSeconds() - totalTimeOut));
                    cumulativeScore += score;

                    SaveManager::addPoints(grid.calculatePoints(score, timer.getElapsedTime().asSeconds() - totalTimeOut));

                    // Saves the new game to the records vector, and saves the records to appdata
                    SaveManager::addRecord(game);
                    SaveManager::saveRecords("records.dat");

                    prevState = stateFlag;
                    stateFlag = STATE_HOME;
                    Grid::eraser_mode = false;
                }
            }

            if (event.type == sf::Event::Resized) {
                updateView(renderTexture, *window); // Updates the window if resized in this frame
            }

            volumeSlider.handleEvent(event, *window);
            brightnessSlider.handleEvent(event, *window);
            contrastSlider.handleEvent(event, *window);
            scrollerSlider.handleEvent(event, *window);
        }

        switch (stateFlag) { // Switch graphical themes depending on the game state
            case STATE_HOME:
                currentTheme = &HOME_THEME;
                exit.setTheme(EXIT_BUTTON);
                break;
            case STATE_EASY:
                currentTheme = &EASY_THEME;
                exit.setTheme(EASY_BUTTON);
                eraser.setTheme(EASY_BUTTON);
                hint.setTheme(EASY_BUTTON);
                break;
            case STATE_MEDIUM:
                currentTheme = &MEDIUM_THEME;
                exit.setTheme(MEDIUM_BUTTON);
                eraser.setTheme(MEDIUM_BUTTON);
                hint.setTheme(MEDIUM_BUTTON);
                break;
            case STATE_HARD:
                currentTheme = &HARD_THEME;
                exit.setTheme(HARD_BUTTON);
                eraser.setTheme(HARD_BUTTON);
                hint.setTheme(HARD_BUTTON);
                break;
            case STATE_KRAZY:
                currentTheme = &KRAZY_THEME;
                exit.setTheme(KRAZY_BUTTON);
                eraser.setTheme(KRAZY_BUTTON);
                hint.setTheme(KRAZY_BUTTON);
                break;
            case STATE_SETTINGS:
                currentTheme = &SETTINGS_THEME;
                exit.setTheme(REGULAR_BUTTON);
                break;
            case STATE_SHOP:
                currentTheme = &SHOP_THEME;
                exit.setTheme(MEDIUM_BUTTON);
                break;
        }

        if (stateFlag >= STATE_EASY && stateFlag <= STATE_KRAZY) { // Gameplay state
            // Deactivating menu buttons
            easySwitch.activateMovement(sf::Vector2f(-210.f,easySwitch.getOriginalPos().y), 600.f);
            mediumSwitch.activateMovement(sf::Vector2f(610.f,mediumSwitch.getOriginalPos().y), 600.f);
            hardSwitch.activateMovement(sf::Vector2f(-210.f,hardSwitch.getOriginalPos().y), 600.f);
            krazySwitch.activateMovement(sf::Vector2f(610.f,krazySwitch.getOriginalPos().y), 600.f);

            easySwitch.deactivate();
            mediumSwitch.deactivate();
            hardSwitch.deactivate();
            krazySwitch.deactivate();

            exit.setText("<-");

            // Activating number choice buttons
            for (Button& button : numberChangers) {
                if (&button != chosenNumber) {
                    button.activate();
                }
            }

            // Activates the grid and its buttons
            grid.activate();
            eraser.activate();
            hint.activate();
            settingsToggle.activate();
            vsyncToggle.deactivate();
            fullscreenToggle.deactivate();
            volumeSlider.deactivate();
            brightnessSlider.deactivate();
            contrastSlider.deactivate();
            scrollerSlider.deactivate();

            shopShelf.deactivate();
        } else if (stateFlag == STATE_HOME){ // Home Menu State
            // Activating menu buttons
            easySwitch.activateMovement(easySwitch.getOriginalPos(), 400.f);
            mediumSwitch.activateMovement(mediumSwitch.getOriginalPos(), 400.f);
            hardSwitch.activateMovement(hardSwitch.getOriginalPos(), 400.f);
            krazySwitch.activateMovement(krazySwitch.getOriginalPos(), 400.f);

            easySwitch.activate();

            if (cumulativeScore >= 1000) {
                mediumSwitch.activate();
            }

            if (cumulativeScore >= 5000) {
                hardSwitch.activate();
            }

            if (cumulativeScore >= 10000) {
                krazySwitch.activate();
            }

            exit.setText("x");

            // Deactivating number choice buttons
            for (Button& button : numberChangers) {
                button.deactivate();
            }

            // Ensures no inputs on the invisible grid can be made when outside gameplay mode
            grid.deactivate();
            eraser.deactivate();
            hint.deactivate();
            settingsToggle.activate();
            vsyncToggle.deactivate();
            fullscreenToggle.deactivate();
            volumeSlider.deactivate();
            brightnessSlider.deactivate();
            contrastSlider.deactivate();
            scrollerSlider.deactivate();

            shopShelf.deactivate();
        } else if (stateFlag == STATE_SETTINGS) { // Settings menu state
            // Deactivating menu buttons
            easySwitch.activateMovement(sf::Vector2f(-210.f,easySwitch.getOriginalPos().y), 600.f);
            mediumSwitch.activateMovement(sf::Vector2f(610.f,mediumSwitch.getOriginalPos().y), 600.f);
            hardSwitch.activateMovement(sf::Vector2f(-210.f,hardSwitch.getOriginalPos().y), 600.f);
            krazySwitch.activateMovement(sf::Vector2f(610.f,krazySwitch.getOriginalPos().y), 600.f);

            easySwitch.deactivate();
            mediumSwitch.deactivate();
            hardSwitch.deactivate();
            krazySwitch.deactivate();

            exit.setText("<-");

            // Deactivating number choice buttons
            for (Button& button : numberChangers) {
                button.deactivate();
            }

            // Ensures no inputs on the invisible grid can be made when outside gameplay mode
            grid.deactivate();
            eraser.deactivate();
            hint.deactivate();
            settingsToggle.deactivate();
            vsyncToggle.activate();
            fullscreenToggle.activate();
            volumeSlider.activate();
            brightnessSlider.activate();
            contrastSlider.activate();
            scrollerSlider.activate();

            shopShelf.deactivate();
        } else { // Shop state
            // Deactivating menu buttons
            easySwitch.activateMovement(sf::Vector2f(-210.f,easySwitch.getOriginalPos().y), 600.f);
            mediumSwitch.activateMovement(sf::Vector2f(610.f,mediumSwitch.getOriginalPos().y), 600.f);
            hardSwitch.activateMovement(sf::Vector2f(-210.f,hardSwitch.getOriginalPos().y), 600.f);
            krazySwitch.activateMovement(sf::Vector2f(610.f,krazySwitch.getOriginalPos().y), 600.f);

            easySwitch.deactivate();
            mediumSwitch.deactivate();
            hardSwitch.deactivate();
            krazySwitch.deactivate();

            exit.setText("<-");

            // Deactivating number choice buttons
            for (Button& button : numberChangers) {
                button.deactivate();
            }

            // Ensures no inputs on the invisible grid can be made when outside gameplay mode
            grid.deactivate();
            eraser.deactivate();
            hint.deactivate();
            settingsToggle.deactivate();
            vsyncToggle.deactivate();
            fullscreenToggle.deactivate();
            volumeSlider.deactivate();
            brightnessSlider.deactivate();
            contrastSlider.deactivate();
            scrollerSlider.deactivate();

            shopShelf.activate();
        }

        music.setVolume(volumeSlider.getPercentage());
        volumeSlider.displayPercentage("","%");
        if (volumeSlider.getDragging()) {
            SaveManager::setVolume(volumeSlider.getPercentage()/100);
        }

        currentBrightness = brightnessSlider.getPercentage()/100.f;
        brightnessSlider.displayPercentage("","%");
        if (brightnessSlider.getDragging()) {
            SaveManager::setBrightness(brightnessSlider.getPercentage()/100);
        }

        contrast = contrastSlider.getPercentage()/100.f;
        contrastSlider.displayPercentage("","%");
        if (contrastSlider.getDragging()) {
            SaveManager::setContrast(contrastSlider.getPercentage()/100);
        }

        scrollSpeed = scrollerSlider.getPercentage()*1.5f;
        scrollerSlider.displayPercentage("","%");
        if (scrollerSlider.getDragging()) {
            SaveManager::setBSpeed(scrollerSlider.getPercentage()/100);
        }

        culScoreText.setString("Total Score: " + std::to_string(SaveManager::getTotalScore()));
        textRect = culScoreText.getLocalBounds();
        culScoreText.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
        culScoreText.setPosition(sf::Vector2f(300.f, 650.f));

        pointsText.setString("Kuukies: " + std::to_string(SaveManager::getPoints()));
        textRect = pointsText.getLocalBounds();
        pointsText.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
        pointsText.setPosition(sf::Vector2f(300.f, 700.f));

        scoreText.setString(std::to_string(score));
        scoreText.setFillColor(currentTheme->text);
        textRect = scoreText.getLocalBounds();
        scoreText.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
        scoreText.setPosition(sf::Vector2f(300.f, 100.f));

        int elapsed = static_cast<int>(timer.getElapsedTime().asSeconds() - totalTimeOut);
        int minutes = elapsed / 60;
        int seconds = elapsed % 60;

        timerText.setString(std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
        timerText.setFillColor(currentTheme->text);
        textRect = timerText.getLocalBounds();
        timerText.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
        timerText.setPosition(sf::Vector2f(300.f, 50.f));

        // Move panels to the right
        float deltaTime = clock.restart().asSeconds();
        panel1.move(scrollSpeed*deltaTime, 0);
        panel2.move(scrollSpeed*deltaTime, 0);
        panel3.move(scrollSpeed*deltaTime, 0);

        bg1.move(scrollSpeed*deltaTime, 0);
        bg2.move(scrollSpeed*deltaTime, 0);
        bg3.move(scrollSpeed*deltaTime, 0);

        // Loop the panels when they go off-screen
        if (panel1.getPosition().x > 600) {
            panel1.setPosition(panel2.getPosition().x - 300, 0);
        }

        if (panel2.getPosition().x > 600) {
            panel2.setPosition(panel3.getPosition().x - 300, 0);
        }

        if (panel3.getPosition().x > 600) {
            panel3.setPosition(panel1.getPosition().x - 300, 0);
        }


        if (bg1.getPosition().x > 750) {
            bg1.setPosition(bg2.getPosition().x - 300, 400);
        }

        if (bg2.getPosition().x > 750) {
            bg2.setPosition(bg3.getPosition().x - 300, 400);
        }

        if (bg3.getPosition().x > 750) {
            bg3.setPosition(bg1.getPosition().x - 300, 400);
        }

        // Set the colors to the theme
        panel1.setFillColor(updateColour(panel1.getFillColor(), currentTheme->bg1, deltaTime));
        panel2.setFillColor(updateColour(panel2.getFillColor(), currentTheme->bg2, deltaTime));
        panel3.setFillColor(updateColour(panel3.getFillColor(), currentTheme->bg3, deltaTime));

        if (stateFlag == STATE_HOME) { // Menu buttons have hover visuals when on the home screen
            easySwitch.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));

            if (cumulativeScore >= 1000) {
                mediumSwitch.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
                mediumSwitch.setTexture(ResourceManager::getTexture("placeholder"), 0.f);
            }

            if (cumulativeScore >= 5000) {
                hardSwitch.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
                hardSwitch.setTexture(ResourceManager::getTexture("placeholder"), 0.f);
            }

            if (cumulativeScore >= 10000) {
                krazySwitch.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
                krazySwitch.setTexture(ResourceManager::getTexture("placeholder"), 0.f);
            }

            shopToggle.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        } else if (stateFlag >= STATE_EASY && stateFlag <= STATE_KRAZY) { // Ensures the buttons have hover visuals when playing the game and not on the home screen
            for (Button& button : numberChangers) {
                if (button.isActive()) {
                    button.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
                } else {
                    button.setColor(button.getTheme().hovering);
                }
            }
            grid.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        } else if (stateFlag == STATE_SETTINGS) { // Settings menu
            vsyncToggle.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
            fullscreenToggle.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        } else { // Shop 
            shopShelf.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        }
        exit.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        if (!Grid::eraser_mode) {
            eraser.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        }
        hint.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));
        settingsToggle.updateHover(window->mapPixelToCoords(sf::Mouse::getPosition(*window), gameView));

        // update the menu buttons each frame for gradual movement
        easySwitch.update(deltaTime);
        mediumSwitch.update(deltaTime);
        hardSwitch.update(deltaTime);
        krazySwitch.update(deltaTime);
        exit.update(deltaTime);
        eraser.update(deltaTime);
        hint.update(deltaTime);
        for (int i = 0; i < 9; i++) {
            numberChangers.at(i).update(deltaTime);
        }
        settingsToggle.update(deltaTime);
        shopToggle.update(deltaTime);
        vsyncToggle.update(deltaTime);
        fullscreenToggle.update(deltaTime);

        // Draw everything necessary
        clearTheme = updateColour(clearTheme, currentTheme->bgClear, deltaTime);
        renderTexture.clear(clearTheme);
        renderTexture.setView(gameView);
        renderTexture.draw(panel1);
        renderTexture.draw(panel2);
        renderTexture.draw(panel3);
        if (bg1.getTexture() != &ResourceManager::getTexture("placeholder")) {
            renderTexture.draw(bg1);
            renderTexture.draw(bg2);
            renderTexture.draw(bg3);
        }
        if (stateFlag >= STATE_EASY && stateFlag <= STATE_KRAZY) { // Draws for an active sudoku game
            grid.display(renderTexture);
            for (Button& button : numberChangers) {
                button.display(renderTexture);
            }
            eraser.display(renderTexture);
            hint.display(renderTexture);
            renderTexture.draw(scoreText);
            renderTexture.draw(timerText);
        } else if (stateFlag == STATE_HOME) { // Draws for the home menu
            renderTexture.draw(title);
            renderTexture.draw(culScoreText);
            renderTexture.draw(pointsText);
            shopToggle.display(renderTexture);
        } else if (stateFlag == STATE_SETTINGS) { // Draws for the settings menu
            renderTexture.draw(settingsTitle);
            renderTexture.draw(volumeText);
            renderTexture.draw(brightnessText);
            renderTexture.draw(contrastText);
            renderTexture.draw(scrollerText);
            vsyncToggle.display(renderTexture);
            fullscreenToggle.display(renderTexture);
            volumeSlider.display(renderTexture);
            brightnessSlider.display(renderTexture);
            contrastSlider.display(renderTexture);
            scrollerSlider.display(renderTexture);
        } else { // Draws for the shop
            shopShelf.display(renderTexture);
        }
        easySwitch.display(renderTexture);
        mediumSwitch.display(renderTexture);
        hardSwitch.display(renderTexture);
        krazySwitch.display(renderTexture);
        exit.display(renderTexture);
        if (settingsToggle.isActive()) {
            settingsToggle.display(renderTexture);
        }
        renderTexture.display();

        // Apply shaders
        window->clear();
        sf::Sprite finalSprite(renderTexture.getTexture());
        shader.setUniform("brightness", currentBrightness);
        shader.setUniform("contrast", contrast);
        window->draw(finalSprite, &shader);
        window->display();
    }
    music.stop();
    return 0;
}
