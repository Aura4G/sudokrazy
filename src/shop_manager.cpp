#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <SFML/System.hpp>
#include <cstdlib>
#include <fstream>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

#include "shop.hpp"
#include "shop_manager.hpp"

std::vector<int> ShopManager::purchases;

std::vector<int> ShopManager::equips;

void ShopManager::save (const std::string& filename, const std::string& data) {
    // The save destination is retrieved
    std::string fullPath = getSavePath(filename);

    // A character buffer stores all of the data...
    std::vector<char> buffer(data.begin(), data.end());

    // ... and encrypts the buffer
    obfuscate(buffer);

    // The encrypted buffer is written to the file
    std::ofstream file(fullPath, std::ios::binary);
    if (file) {
        file.write(buffer.data(), buffer.size());
    }
}

std::string ShopManager::loadSave(const std::string& filename) {
    // The save's destination is retrieved
    std::string fullPath = getSavePath(filename);

    // No records are created if the file doesn't exists
    std::ifstream file (fullPath, std::ios::binary);
    if (!file) return "";

    // A buffer captures the file's contents and decrypts them
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), {});
    obfuscate(buffer);

    return std::string(buffer.begin(), buffer.end());
}

void ShopManager::obfuscate(std::vector<char>& buffer) {
    const char key = 0x5A; // XOR key used for encrypting and decrypting
    for (auto& c : buffer) {
        c ^= key;
    }
}

std::string ShopManager::getSavePath(const std::string& filename) {
    namespace fs = std::filesystem;

    // Static variable ensures this block runs only once
    static fs::path saveDir = []() -> fs::path {
        fs::path path;

    #ifdef _WIN32 // Windows, saved in Roaming
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

        // Creates the directory if it doesn't already exist
        if (!fs::exists(path)) {
            fs::create_directories(path);
        }

        return path;
    }();

    // Combine with filename
    fs::path fullPath = saveDir / filename;
    return fullPath.string();
}

void ShopManager::saveShop(const std::string& filename, int ID) {

    purchases.push_back(ID);

    // String stream to format purchased/equipped item IDs into comma-separated strings
    std::stringstream ss;

    for (int i = 0; i < purchases.size(); ++i) {
        ss << purchases[i];
        if (i < purchases.size() - 1) {
            ss << ",";
        }
    }

    // Store the comma-separated IDs into a string that can be written to a .dat file
    std::string purchasesString = ss.str();

    // Clear the stream to do the same for equippables
    ss.clear();

    for (int i = 0; i < equips.size(); ++i) {
        ss << equips[i];
        if (i < equips.size() - 1) {
            ss << ",";
        }
    }

    // Store comma-separated IDs into a string
    std::string equipsString = ss.str();


    std::ostringstream out;

    //delimits values with new lines in a specific order
    out << purchasesString << "\n"
        << equipsString;

    ShopManager::save(filename, out.str());    
}

void ShopManager::loadShop(const std::string& filename) {
    purchases.clear();
    equips.clear();

    std::string data = loadSave(filename);
    std::istringstream in(data);
    std::string line;

    // Purchased items
    if (std::getline(in, line)) {
        std::istringstream ls(line);
        std::string field;
        while (std::getline(ls, field, ',')) { 
            purchases.push_back(std::stoi(field));
        }
    }

    // Equipped items
    if (std::getline(in, line)) {
        std::istringstream ls(line);
        std::string field;
        while (std::getline(ls, field, ',')) {
            equips.push_back(std::stoi(line));
        }
    }
}

std::vector<int> ShopManager::getPurchases() {
    return purchases;
}

std::vector<int> ShopManager::getEquips() {
    return equips;
}

bool ShopManager::queryPurchases(int ID) {
    bool found = std::find(purchases.begin(), purchases.end(), ID) != purchases.end();

    return found;
}