#ifndef __RECORD_HPP__
#define __RECORD_HPP__

#include <vector>
#include <SFML/System.hpp>
#include "sudoku.hpp"

/**
 * These classes provide the means for which players can log play-through efforts
 * As well as accumulate score over the course of multiple play sessions
 * 
 * @author Aura4G (Aria Noroozi)
*/


/**
 * Encapsulates the score metric of the game, along with the relevant data of a sudoku game playthrough
*/
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

private:

    //The accrued score of the game
    int score;

    //The difficulty played on
    GameState difficulty;

    //The time taken to correctly complete the board
    sf::Time time;

};

class RecordManager {
public:

    /// @return a completed game's record
    static Record getRecord(int index);

    //Adds a record to the records vector
    static void addRecord(Record game);

private:

    //Vector storing all completed sudoku game instances, with their scores and times
    static std::vector<Record> records;
};

#endif