#ifndef __SHOP_HPP__
#define __SHOP_HPP__

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "slider.hpp"

/**
 * This file and collection of classes handles the creation, usage and selection
 * of items found in the shop
 * 
 * This presents an example of how other classes can mold the button object
 * for their own use, creating useful structures
 * 
 * @author Aura4G (Aria Noroozi)
*/

/// @brief Specifies the type of reward gained from an item
enum class ItemType {Background, Music, SFX, Hint};

/// @brief Stores all of the data regarding a purchasable
class Item {
public:

    /// @brief Default constructor
    Item();

    /**
     * @brief Parameterised constructor to build an item with unique ID
     * @param cost The item's cost to purchase
     * @param name The item's name
     * @param texture The item's preview image in the shop
     * @param equipKey Utilised to find specific media in the resource manager the item unlocks/equips.
     * @param type The type of item it is
    */
    Item(int cost, std::string name, std::string texture = "placeholder", std::string equipKey = "hint", ItemType type = ItemType::Hint);

    /// @return The item's ID
    int getID() const;

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

    /// @return The preview key of the item
    std::string getPreviewKey();

    /// @param newPreview The new preview image displayed for the unlockable item
    void setPreview(std::string& key);

    /// @return The type of the item
    ItemType getType();

    /// @return The purchase status of the item
    bool isPurchased();

    /// @brief Sets purchased to true
    void setPurchased(bool val=true);

    /**
     * @brief Purchases the item with earnt points
     * @return True if the transaction goes through, false otherwise.
    */
    bool purchase();

    /// @return The equip status of the item
    bool isEquipped();

    void setEquip(bool val=true);

    void changeEquips(bool val = true);

    void setEquipKey(std::string newKey);

    std::string getEquipKey();

private:

    /// @brief the unique ID of the item 
    int itemID;

    /// @brief How much the player will have to pay with in-game currency to get the item
    int cost;

    /// @brief The name of the item
    std::string name;

    /// @brief The description of the item
    std::string description;

    /// @brief The preview image that appears
    std::string previewKey;

    /// @brief Whether or not the item has been purchased
    bool purchased;

    /// @brief Whether or not the item is currently equipped
    bool equipped;

    /// @brief String variable to be plugged into resource manager queries to find the right media for the equip
    std::string equipKey;

    /// @brief increments and assigns with each item creation
    static int counter;

    /// @brief The type of item being sold
    ItemType type;
};


/// @brief Handler for all the items in the shop
class Shop {
public:

    /**
     * @brief Adds an item object to the items vector
     * @param item The newly created item
    */
    static void addItem(Item item);

    /**
     * @brief Retrieves an item registered within the shop via its name
     * @param name The name of the item
     * @return An item with a matching name
    */
    static Item getItem(std::string name);

    /**
     * @brief Retrieves an item registered within the shop via its index in the vector of items
     * @param index The item's index
     * @return The item of that index
    */
    static std::optional<Item> getItem(int index);

    static std::optional<Item> getItemByID(int ID);

    /// @return The number of items in the shop
    static int getSize();

    /// @return The total number of items in the shop
    static int getTotalItems();

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

    /**
     * @brief takes the shop's contents from an index and applies them to the visualised shop shelf
     *        until the shelf's limit is reached or the shop runs out of items from that index
     * @param index the starting point for which the shelf is stocked with items
    */
    void pullShop(int index = 0);

    /**
     * @brief Iteratively calls the update-hover function for every item display in the shelf
     * @param mousePos Mouse position relative to the window in the current frame
    */
    void updateHover(const sf::Vector2f& mousePos);

    /// @brief Activates all the player-interactive buttons in the shelf
    void activate();

    /// @brief Deactivates all the player-interactive buttons in the shelf
    void deactivate();

    /**
     * @brief Updates the purchase and equip status of items on the shelf
     *        whenever a left-click event occurs
     * @param mousePos The position of the mouse during that frame
    */
    bool updateShelf(const sf::Vector2f& mousePos);

    /// @brief Updates the hint total text under the hint purchase button of the shop shelf
    void updateHintCount();

    /**
     * @brief Handles the slider's button movement and bounds according to player-triggered events
     * @param event the polled event passed from main, where the window is being rendered
     * @param window the rendered window the game is running on 
    */
    void scrollShelf(const sf::Event& event, sf::RenderWindow& window);

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

    /// @brief Vector storing the item data for each item on the shelf
    std::vector<Item> itemsPulled;

    /// @brief the number of times the quantity of shop items overflows the dimensions of the shelf
    int sections;

    /// @brief Horizontal scroller for the shop shelf
    Slider shopScroller;

    /// @brief The X-position of the rightmost button on the top layer of the shop shelf visible when starting up the game
    float initScreenRightmost;

    /// @brief The X-position of the rightmost button of the entire shop shelf
    float globalRightmost;

    /**
     * @brief Toggles the caption section of a button to become the equip/equipped functionality button
     * @param caption The button becoming the equip toggle
     * @param val True if the item is equipped, false otherwise
    */
    void toggleCaption(Button& caption, bool val);
};

#endif