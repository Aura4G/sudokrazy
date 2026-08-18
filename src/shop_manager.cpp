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

std::vector<int> ShopManager::equipIDs;

std::vector<Item> ShopManager::equips;

int ShopManager::hints;

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

void ShopManager::savePurchase(const std::string& filename, Item& item) {

    if (item.getType() == ItemType::Hint) {
        hints++; // The hint item type is not added towards the vector of purchases; they're expendables to be used during sudoku games
    } else {
        purchases.push_back(item.getID()); // Purchased item pushed to the back of the vector

        // Flag becomes true to signify if an equippable replacement is made
        bool flag = false;

        // When purchasing a particular item type, if the player already has an item of that type equipped, said equipped item is replaced
        // by the new item.
        for (int i = 0; i < equipIDs.size(); i++) {
            int comparator = equipIDs.at(i);
            Item compareItem = equips.at(i);

            if (item.getType() == compareItem.getType() && comparator != item.getID()) {
                std::replace(equipIDs.begin(), equipIDs.end(), comparator, item.getID());
                std::replace_if(equips.begin(), equips.end(), [&](const Item& obj) { return obj.getID() == comparator; }, item);
                flag = true;
                break;
            }
        }
        
        // If it's the first of that type of item, it just gets pushed to the back of the equips vector.
        if (!flag) {
            equipIDs.push_back(item.getID());
            equips.push_back(item);
        }
    }

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
    ss.str("");
    ss.clear();

    for (int i = 0; i < equipIDs.size(); ++i) {
        ss << equipIDs[i];
        if (i < equipIDs.size() - 1) {
            ss << ",";
        }
    }

    // Store comma-separated IDs into a string
    std::string equipsString = ss.str();


    std::ostringstream out;

    //delimits values with new lines in a specific order
    out << purchasesString << "\n"
        << equipsString << "\n"
        << std::to_string(hints);

    ShopManager::save(filename, out.str());    
}

void ShopManager::saveInfo(const std::string& filename) {
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
    ss.str("");
    ss.clear();

    for (int i = 0; i < equipIDs.size(); ++i) {
        ss << equipIDs[i];
        if (i < equipIDs.size() - 1) {
            ss << ",";
        }
    }

    // Store comma-separated IDs into a string
    std::string equipsString = ss.str();


    std::ostringstream out;

    //delimits values with new lines in a specific order
    out << purchasesString << "\n"
        << equipsString << "\n"
        << std::to_string(hints);

    ShopManager::save(filename, out.str());  
}

void ShopManager::changeEquips(Item& item) {
    // Flag becomes true to signify if an equippable replacement is made
    bool flag = false;

    // When purchasing a particular item type, if the player already has an item of that type equipped, said equipped item is replaced
    // by the new item.
    for (int i = 0; i < equipIDs.size(); i++) {
        int comparator = equipIDs.at(i);
        Item compareItem = equips.at(i);

        if (item.getType() == compareItem.getType() && comparator != item.getID()) {
            std::replace(equipIDs.begin(), equipIDs.end(), comparator, item.getID());
            std::replace_if(equips.begin(), equips.end(), [&](const Item& obj) { return obj.getID() == comparator; }, item);
            flag = true;
            break;
        }
    }
        
    // If it's the first of that type of item, it just gets pushed to the back of the equips vector.
    if (!flag) {
        equipIDs.push_back(item.getID());
        equips.push_back(item);
    }
}

void ShopManager::loadShop(const std::string& filename) {
    purchases.clear();
    equipIDs.clear();

    std::string data = loadSave(filename);
    std::istringstream in(data);
    std::string line;

    // Purchased item IDs
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string field;
        while (std::getline(iss, field, ',')) { 
            purchases.push_back(std::stoi(field));
        }
    }

    // Equipped item IDs
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string field;
        while (std::getline(iss, field, ',')) {
            equipIDs.push_back(std::stoi(line));
        }
    }

    // Hint count
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> hints;
    }

    // Items Vector
    for (int ID : equipIDs) {
        std::optional<Item> item = Shop::getItemByID(ID);

        if (item) {
            equips.push_back(item.value());
        }
    }
}

std::vector<int> ShopManager::getPurchases() {
    return purchases;
}

std::vector<int> ShopManager::getEquips() {
    return equipIDs;
}

bool ShopManager::queryPurchases(int ID) {
    bool found = std::find(purchases.begin(), purchases.end(), ID) != purchases.end();

    return found;
}

bool ShopManager::queryEquips(int ID) {
    bool found = std::find(equipIDs.begin(), equipIDs.end(), ID) != equipIDs.end();

    return found;
}

void ShopManager::alterHints(int additive) {
    hints += additive;
}

int ShopManager::getHints() {
    return hints;
}