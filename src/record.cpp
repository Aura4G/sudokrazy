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
float SaveManager::volume = 1.f;
float SaveManager::brightness = 1.f;
float SaveManager::backgroundSpeed = 0.5f;
bool SaveManager::vsync = true;
bool SaveManager::fullscreen = false;
int SaveManager::points = 0;


/// Record Class definitions ///

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


/// Save Manager definitions ///

Record SaveManager::getRecord(int index) {
    return records.at(index);
}

void SaveManager::addRecord(Record game) {
    records.emplace_back(game);
}

void SaveManager::save (const std::string& filename, const std::string& data) {
    //The save destination is retrieved
    std::string fullPath = getSavePath(filename);

    //A character buffer stores all of the data...
    std::vector<char> buffer(data.begin(), data.end());

    //... and encrypts the buffer
    obfuscate(buffer);

    //the encrypted buffer is written to the file
    std::ofstream file(fullPath, std::ios::binary);
    if (file) {
        file.write(buffer.data(), buffer.size());
    }
}

std::string SaveManager::loadSave(const std::string& filename) {
    //The save's destination is retrieved
    std::string fullPath = getSavePath(filename);

    //No records are created if the file doesn't exists
    std::ifstream file (fullPath, std::ios::binary);
    if (!file) return "";

    //A buffer captures the file's contents and decrypts them
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), {});
    obfuscate(buffer);

    return std::string(buffer.begin(), buffer.end());
}

void SaveManager::obfuscate(std::vector<char>& buffer) {
    const char key = 0x5A; // A given XOR key used for encrypting and decrypting
    for (auto& c : buffer) {
        c ^= key;
    }
}

std::string SaveManager::getSavePath(const std::string& filename) {
    namespace fs = std::filesystem;

    // Static variable ensures this block runs only once
    static fs::path saveDir = []() -> fs::path {
        fs::path path;

    #ifdef _WIN32 //Windows, saved in Roaming
        const char* appData = getenv("APPDATA");
        if (!appData) appData = ".";
        path = fs::path(appData) / "Sudokrazy";
    #elif __APPLE__ //MacOS
        const char* home = getenv("HOME");
        path = fs::path(home) / "Library" / "Application Support" / "Sudokrazy";
    #else // Linux/Unix
        const char* home = getenv("HOME");
        path = fs::path(home) / ".local" / "share" / "sudokrazy";
    #endif

        //Creates the directory if it doesn't already exist
        if (!fs::exists(path)) {
            fs::create_directories(path);
        }

        return path;
    }();

    // Combine with filename
    fs::path fullPath = saveDir / filename;
    return fullPath.string();
}

void SaveManager::saveRecords(const std::string& filename) {
    std::ostringstream out;
    for (auto& r : records) {
        std::string s = r.serialise(); //serialise each value in each record to a single string
        out << s.size() << "\n" << s << "\n"; //Each value is delimited by a new line
    }

    out << "points " << std::to_string(points);

    save(filename, out.str());
}

void SaveManager::loadRecords(const std::string& filename) {
    records.clear();
    std::string data = loadSave(filename);
    std::istringstream in(data);

    while (true) {
        std::size_t len;
        if (!(in >> len)) break; // not a number, therefore it must be the last line
        in.get(); // skip newline

        std::string recordString(len, '\0');
        in.read(&recordString[0], len);
        in.get(); // skip newline after record

        records.emplace_back(Record::deserialize(recordString)); //Add record to vector
    }

    // now just read the last line as an integer
    in.clear(); // reset stream state after the failed number read
    
    // Read the footer line
    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("points ", 0) == 0) {
            std::istringstream ls(line.substr(7)); //Have the stringstream continue from directly after the "points" text
            ls >> points;
        }
    }
}

void SaveManager::saveSettings(const std::string& filename) {
    std::ostringstream out;

    //delimits values with new lines in a specific order
    out << std::to_string(volume) << "\n"
        << std::to_string(brightness) << "\n"
        << std::to_string(backgroundSpeed) << "\n"
        << std::to_string(vsync) << "\n"
        << std::to_string(fullscreen);

    save(filename, out.str());
}

void SaveManager::loadSettings(const std::string& filename) {
    std::string data = loadSave(filename);

    //If there's no current settings configuration to load, a default is used instead
    if (data == "") {
        data = "1\n1\n0.5\n1\n0";
    }

    std::istringstream in(data);

    std::string line;

    // Volume
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> volume;
    }

    // Brightness
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> brightness;
    }

    // Background scroll speed
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> backgroundSpeed;
    }

    // V-Sync
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> vsync;
    }

    // Fullscreen
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> fullscreen;
    }
}

float SaveManager::getVolume() {
    return volume;
}

void SaveManager::setVolume(float newVolume) {
    volume = newVolume;
}

float SaveManager::getBrightness() {
    return brightness;
}

void SaveManager::setBrightness(float newBrightness) {
    brightness = newBrightness;
}

float SaveManager::getBSpeed() {
    return backgroundSpeed;
}

void SaveManager::setBSpeed(float newSpeed) {
    backgroundSpeed = newSpeed;
}

bool SaveManager::getVSync() {
    return vsync;
}

void SaveManager::setVSync(bool flag) {
    vsync = flag;
}

bool SaveManager::getFullscreen() {
    return fullscreen;
}

void SaveManager::setFullscreen(bool flag) {
    fullscreen = flag;
}

void SaveManager::addPoints(int newPoints) {
    points += newPoints;
}

int SaveManager::getPoints() {
    return points;
}