#ifndef __SHOP_HPP__
#define __SHOP_HPP__

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "button.hpp"

/**
 * This file and collection of classes handles the creation, usage and selection
 * of items found in the shop
 * 
 * This presents an example of how other classes can mold the button object
 * for their own use, creating useful structures
 * 
 * @author Aura4G (Aria Noroozi)
*/

/// @brief Stores all of the data regarding a purchasable
class Item {
public:

    /**
     * @brief Parameterised constructor
     * @param cost The item's cost to purchase
     * @param name The item's name
     * @param description The item's description
    */
    Item(int cost, std::string& name, std::string& description);

    /// @return The item's ID
    int getID();

    /// @return The cost of the item
    int getCost();

    /// @param newCost The new cost of the item
    void setCost(int newCost);

    /// @return The name identifier of the item
    std::string getName();

    /// @param newName The name the item will assume
    void setName(std::string& newName);

    /// @return The description of the item
    std::string getDescription();

    /// @param newDesc The new description of the item
    void setDescription(std::string& newDesc);

    /// @return The purchase status of the item
    bool isPurchased();

    /// @brief Purchases the item with earnt points
    void purchase();

private:

    /// @brief the unique ID of the item 
    int itemID;

    /// @brief How much the player will have to pay with in-game currency to get the item
    int cost;

    /// @brief The name of the item
    std::string name;

    /// @brief The description of the item
    std::string description;

    /// @brief Whether or not the item has been purchased
    bool purchased;

    /// @brief increments and assigns with each item creation
    static int counter;
};


/// @brief Handler for all the items in the shop
class Shop {
public:

    /**
     * @brief Adds an item object to the items map
     * @param item The newly created item
    */
    static void addItem(Item item);

    /**
     * @brief Retrieves an item registered within the shop via its name
     * @param name The name of the item
     * @return An item with a matching key
    */
    static Item getItem(std::string name);

private:

    /// @brief Every purchasable item available in the shop
    static std::vector<Item> items;
};


/// @brief Displays an array of buttons representing purchasables at a time, shifting with player input
class Shelf {
public:

    /**
     * @brief parameterised constructor for item shelves
     * @param columns the number of columns in the item selection
     * @param rows the number of rows in the item selection
     * @param position the draw position of the shelf
     * @param displaySize the size of one item display
     * @param gaps gaps between each display items
    */
    Shelf(int columns, int rows, sf::Vector2f position, sf::Vector2f displaySize, sf::Vector2f gaps);

    /**
     * @brief Calls all necessary draws and displays for each button in the shelf
     * @param renderTexture The texture being drawn to
    */
    void display(sf::RenderTexture& renderTexture);

private:

    /// @brief How many item columns are displayed along the x axis
    int columns;

    /// @brief How many item rows are displayed along the y axis
    int rows;

    /// @brief The location of the top left of the shelf
    sf::Vector2f position;

    /// @brief The size of an item purchase button
    sf::Vector2f displaySize;

    /// @brief The x and y gaps between each display item on the shelf
    sf::Vector2f gaps;

    /// @brief The items on display
    std::vector<Button> itemDisplay;

    /// @brief The captions of the items on display
    std::vector<Button> itemCaption;
};

#endif