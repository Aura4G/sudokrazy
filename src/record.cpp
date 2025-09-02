#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <SFML/System.hpp>
#include <cstdlib>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif


#include "record.hpp"
#include "sudoku.hpp"

std::vector<Record> SaveManager::records;

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

std::string Record::serialise() const {
    std::ostringstream out;
    out << score << "\n"
        << static_cast<int>(difficulty) << "\n"
        << time.asMilliseconds();
    return out.str();
}

Record Record::deserialize(const std::string& str) {
    std::istringstream in(str);
    int score;
    int difficulty;
    sf::Int32 msTime;

    in >> score >> difficulty >> msTime;

    return Record(score, static_cast<GameState>(difficulty), sf::milliseconds(msTime));
}




Record SaveManager::getRecord(int index) {
    return records.at(index);
}

void SaveManager::addRecord(Record game) {
    records.emplace_back(game);
}

void SaveManager::Save (const std::string& filename, const std::string& data) {
    std::string fullPath = GetSavePath(filename);

    std::vector<char> buffer(data.begin(), data.end());
    Obfuscate(buffer);

    std::ofstream file(fullPath, std::ios::binary);
    if (file) {
        file.write(buffer.data(), buffer.size());
    }
}

std::string SaveManager::loadSave(const std::string& filename) {
    std::string fullPath = GetSavePath(filename);

    std::ifstream file (fullPath, std::ios::binary);
    if (!file) return "";

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), {});
    Obfuscate(buffer);

    return std::string(buffer.begin(), buffer.end());
}

void SaveManager::Obfuscate(std::vector<char>& buffer) {
    const char key = 0x5A;
    for (auto& c : buffer) {
        c ^= key;
    }
}

std::string SaveManager::GetSavePath(const std::string& filename) {
    namespace fs = std::filesystem;

    // Static variable ensures this block runs only once
    static fs::path saveDir = []() -> fs::path {
        fs::path path;

#ifdef _WIN32
        const char* appData = getenv("APPDATA");
        if (!appData) appData = ".";
        path = fs::path(appData) / "Sudokrazy";
#elif __APPLE__
        const char* home = getenv("HOME");
        path = fs::path(home) / "Library" / "Application Support" / "Sudokrazy";
#else // Linux/Unix
        const char* home = getenv("HOME");
        path = fs::path(home) / ".local" / "share" / "sudokrazy";
#endif

        if (!fs::exists(path)) {
            fs::create_directories(path);
        }

        return path;
    }();

    // Combine with filename
    fs::path fullPath = saveDir / filename;
    return fullPath.string();
}

void SaveManager::SaveRecords(const std::string& filename) {
    std::ostringstream out;
    for (auto& r : records) {
        std::string s = r.serialise();
        out << s.size() << "\n" << s;
    }
    Save(filename, out.str());
}

void SaveManager::LoadRecords(const std::string& filename) {
    records.clear();
    std::string data = loadSave(filename);
    std::istringstream in(data);

    while (in) {
        std::size_t len;
        in >> len;
        in.get();

        if (!in) break;

        std::string recordString(len, '\0');
        in.read(&recordString[0], len);
        records.emplace_back(Record::deserialize(recordString));
    }
}