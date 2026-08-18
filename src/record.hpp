#ifndef __RECORD_HPP__
#define __RECORD_HPP__

#include <vector>
#include <SFML/System.hpp>
#include "sudoku.hpp"

/**
 * These classes provide the means for which players can log play-through efforts and various system
 * settings and variables, as well as accumulate score and points over the course of multiple play sessions
 * 
 * @author Aura4G (Aria Noroozi)
*/


/// @brief Encapsulates the score metric of the game, along with the relevant data of a sudoku game playthrough
class Record {
public:

    /**
     * Parameterised constructor, called for a completed game to store the game's data
     * @param score The score of the finished game
     * @param difficulty The difficulty of the finished game
     * @param time The player's completion time
    */
    Record(int score, GameState difficulty, sf::Time time);

    /// @return The score of the inquired game
    int getScore();

    /// @return The difficulty of the inquired game
    GameState getDifficulty();

    /// @return The time of the inquired game
    sf::Time getTime();

    /**
     * @brief converts the data to a string stream
     * @return the string of data now to be obfuscated and written
    */
    std::string serialise() const;

    /**
     * @brief Converts a string of information into each relevant attribute of a record object
     * @param str The decrypted string read from the save file
     * @return A record object with that string's information
    */
    static Record deserialize(const std::string& str);

private:

    /// @brief The accrued score of the game
    int score;

    /// @brief The difficulty played on
    GameState difficulty;

    /// @brief The time taken to correctly complete the board
    sf::Time time;

};

class SaveManager {
public:

    /// GAME RECORDS ///

    /// @return a completed game's record
    static Record getRecord(int index);

    /**
     * @brief Adds a record to the records vector
     * @param game the newly completed sudoku game
    */
    static void addRecord(Record game);

    /**
     * @brief Formats record objects and parses into a save.dat 
     * @param filename the name of the .dat file in the app data folder
    */
    static void saveRecords(const std::string& filename);

    /**
     * @brief Fills up the records vector from a save.dat 
     * @param filename the name of the .dat file in the app data folder
    */
    static void loadRecords(const std::string& filename);

    /// @return The cumulative score across all recorded games in the records vector
    static int getTotalScore();

    /// SETTINGS ///

    /**
     * @brief Formats the settings variables and parses into a save.dat
     * @param filename the name of the .dat file in the app data folder
    */
    static void saveSettings(const std::string& filename);

    /**
     * @brief Sets the settings variable values from a save.dat
     * @param filename the name of the .dat file in the app data folder
    */
    static void loadSettings(const std::string& filename);

    /**
     * @brief Used when starting the game, to set the volume specified in the save file
     * @return The volume of the music
    */
    static float getVolume();

    /**
     * @brief Changes the volume variable that'll be present in the settings save
     * @param newVolume the new volume value
    */
    static void setVolume(float newVolume);

    /**
     * @brief Used when starting the game, to set the brightness specified in the save file
     * @return The brightness of the game winow
    */
    static float getBrightness();

    /**
     * @brief Changes the brightness variable that'll be present in the settings save
     * @param newBrightness the new brightness value
    */
    static void setBrightness(float newBrightness);

    /**
     * @brief Used when starting the game, to set the contrast specified in the save file
     * @return The contrast of the game window
    */
    static float getContrast();

    /**
     * @brief Changes the contrast variable that'll be present in the settings save
     * @param newContrast the new contrast value
    */
    static void setContrast(float newContrast);

    /**
     * @brief Used when starting the game, to set the background scroll speed specified in the save file
     * @return The scroll speed of the background
    */
    static float getBSpeed();

    /**
     * @brief Changes the backgroundSpeed variable that'll be present in the settings save
     * @param newSpeed the new scroll speed value
    */
    static void setBSpeed(float newSpeed);

    /**
     * @brief Used when starting the game, to set the vsync based on the save file
     * @return Whether or not V-Sync was activated in the save file
    */
    static bool getVSync();

    /**
     * @brief Toggles the vsync variable that'll be present in the settings save
     * @param flag The new vsync value
    */
    static void setVSync(bool flag);

    /**
     * @brief Used when starting the game, to set the window type based on the save file
     * @return Whether or not fullscreen was activated in the save file
    */
    static bool getFullscreen();

    /**
     * @brief Toggles the fullscreen variable that'll be present in the settings save
     * @param flag The new fullscreen value
    */
    static void setFullscreen(bool flag);


    /// METRICS ///

    /**
     * @brief Adds/subtracts points from the save manager's points counter
     * @param newPoints The points being added to the total, use negative ints to subtract
     *                  from the total
    */
    static void addPoints(int newPoints);

    /// @return The number of points the player currently has
    static int getPoints();

private:

    //Vector storing all completed sudoku game instances, with their scores and times
    static std::vector<Record> records;

    /**
     * @brief Applies an XOR encryption on the string data to hide it from the player in the .dat file.
     * 
     * Inversely, the same XOR key is used to decrypt the data when loading the .dat file.
     * 
     * @param buffer The string stream containing all of the information loaded/to-be-stored
    */
    static void obfuscate(std::vector<char>& buffer);

    /**
     * @brief Finds the file path for the game's saves, depending on the operating system
     * @param filename The name of the file being found/made in the path
     * @return The file path
    */
    static std::string getSavePath(const std::string& filename);

    /**
     * @brief Parses the object/variable data to a buffer, obfuscates it, and writes it to a .dat file
     * @param filename The name of the file being saved to/created
     * @param data the data being saved
    */
    static void save(const std::string& filename, const std::string& data);

    /**
     * @brief Retrieves and decrypts a save file
     * @param filename The name of the file being loaded
    */
    static std::string loadSave(const std::string& filename);

    /// Settings ///

    /// @brief The volume of the music
    static float volume;

    /// @brief The brightness of the window
    static float brightness;

    /// @brief The colour contrast of the window
    static float contrast;

    /// @brief The speed of the scrolling background
    static float backgroundSpeed;

    /// @brief Indicates V-Sync activation
    static bool vsync;

    ///@brief Indicates Window mode; fullscreen or windowed
    static bool fullscreen;


    /// Metrics ///
    /// @brief The in-game currency "kuukies"
    static int points;
};

#endif