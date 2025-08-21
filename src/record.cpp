#include <vector>
#include <SFML/System.hpp>
#include "record.hpp"
#include "sudoku.hpp"

std::vector<Record> RecordManager::records;

Record::Record(int score, GameState difficulty, sf::Time time)
    : score(score), difficulty(difficulty), time(time)
    {

    }

int Record::getScore() {
    return score;
}

GameState Record::getDifficulty() {
    return difficulty;
}

sf::Time Record::getTime() {
    return time;
}


Record RecordManager::getRecord(int index) {
    return records.at(index);
}

void RecordManager::addRecord(Record game) {
    records.emplace_back(game);
}