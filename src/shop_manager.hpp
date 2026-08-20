#ifndef __SHOP_MANAGER_HPP__
#define __SHOP_MANAGER_HPP__

#include <vector>
#include <string>
#include <SFML/System.hpp>

#include "shop.hpp"

/**
 * The shop manager class provides the means for which shop items are logged as purchased upon the opening and closing of a game,
 * as well as loading the same items upon reload.
 * 
 * @author Aura4G (Aria Noroozi)
*/

/// @brief Manages purchases and equipped items across multiple sittings.
class ShopManager {
public:

    /**
     * @brief Saves the purchase information of the shop
     * @param filename Name of file to save in app data
     * @param item The item being purchased
    */
    static void savePurchase(const std::string& filename, Item& item);

    /**
     * @brief Saves shop information
     * @param filename Name of file to save in app data
    */
    static void saveInfo(const std::string& filename);

    /**
     * @brief Changes equips vector according to the selected item
     * @param item The item being equipped.
    */
    static void changeEquips(Item& item);

    /**
     * @brief Removes an item from the equips vector
     * @param item The item being removed
    */
    static void removeEquip(Item& item);

    /**
     * @brief Loads the purchase & equip information of the shop, preventing the player from paying for the same item twice
     * @param filename Name of file to save in app data
    */
    static void loadShop(const std::string& filename);

    /// @return A the vector of integers representing purchased item IDs
    static std::vector<int> getPurchases();

    /// @return A the vector of integers representing equipped item IDs
    static std::vector<int> getEquipIDs();

    /// @return A vector of the "equipped" items
    static std::vector<Item> getEquips();

    /**
     * @brief Finds a specific item ID in the purchases vector to see if the player already has this item
     * @param ID The item's ID
     * @return true if the ID is present in the vector, false otherwise
    */
    static bool queryPurchases(int ID);

    /**
     * @brief Finds a specific item ID in the equips vector to see if the player was equipping the item
     * @param ID The item's ID
     * @return true if the ID is present in the vector, false otherwise
    */
    static bool queryEquips(int ID);

    /**
     * @brief Change the number of hints the player has
     * @param additive The number of hints being added to the player's total hints. Negative values reduce hint count
    */
    static void alterHints(int additive);

    /// @return The total number of hints the player has
    static int getHints();

private:

    /// @brief Vector of all purchased items
    static std::vector<int> purchases;

    /// @brief Vector of all equipped items IDs
    static std::vector<int> equipIDs;

    /// @brief Vector of all equipped items
    static std::vector<Item> equips;

    /// @brief Purchased hint tokens
    static int hints;

    /**
     * @brief Writes string data to .dat file
     * @param filename Name of file to save in app data
     * @param data The formatted data
    */ 
    static void save(const std::string& filename, const std::string& data);

    /**
     * @brief Retrieves and decrypts a save file
     * @param filename The name of the file being loaded
    */
    static std::string loadSave(const std::string& filename);

    /**
     * @brief Finds the file path for the game's saves, depending on the operating system
     * @param filename The name of the file being found/made in the path
     * @return The file path
    */
    static std::string getSavePath(const std::string& filename);

    /**
     * @brief Applies an XOR encryption/decryption on the string data to hide/reveal it from/to the player in the .dat file.
     * @param buffer The string stream containing all of the information loaded/to-be-stored
    */
    static void obfuscate(std::vector<char>& buffer);
};

#endif