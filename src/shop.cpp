#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <optional>

#include <SFML/Graphics.hpp>

#include <sudokrazy/shop.hpp>
#include <sudokrazy/record.hpp>
#include <sudokrazy/sudoku.hpp>
#include <sudokrazy/button.hpp>
#include <sudokrazy/resource_manager.hpp>
#include <sudokrazy/shop_manager.hpp>
#include <sudokrazy/main.hpp>

#include <rapidcsv/rapidcsv.h>

int Item::counter = 0;

std::vector<Item> Shop::items;

/// Item class definitions ///

Item::Item() {
    cost = 0;
    name = "placeholder";
    description = "";
    itemID = -1;
    previewKey = "placeholder";
    type = ItemType::Hint;
    purchased = false;
}

Item::Item(int cost, std::string name, std::string texture, std::string equipKey, ItemType type)
    : cost(cost), name(name), equipKey(equipKey), type(type)
    {
        purchased = false;
        equipped = false;
        itemID = counter++;
        previewKey = texture;

        description = name + "\nCost: " + std::to_string(cost);
    }

int Item::getID() const {
    return itemID;
}

int Item::getCost() {
    return cost;
}

void Item::setCost(int newCost) {
    cost = newCost;
}

std::string Item::getName() {
    return name;
}

void Item::setName(std::string& newName) {
    name = newName;
}

std::string Item::getDescription() {
    return description;
}

void Item::setDescription(std::string& newDesc) {
    description = newDesc;
}

std::string Item::getPreviewKey() {
    return previewKey;
}

void Item::setPreview(std::string& key) {
    previewKey = key;
}

ItemType Item::getType() {
    return type;
}

bool Item::isPurchased() {
    return purchased;
}

void Item::setPurchased(bool val) {
    purchased = val;
}

bool Item::purchase() {
    if (cost <= SaveManager::getPoints() && !purchased) {
        SaveManager::addPoints(-cost);
        SaveManager::saveRecords("records.dat");
        purchased = (type == ItemType::Hint) ? false : true;
        ShopManager::savePurchase("shop.dat", *this);

        return true;
    }

    return false;
}

bool Item::isEquipped() {
    return equipped;
}

void Item::setEquip(bool val) {
    equipped = val;
}

void Item::changeEquips(bool val) {
    equipped = val;
    ShopManager::changeEquips(*this);
}

void Item::setEquipKey(std::string newKey) {
    equipKey = newKey;
} 

std::string Item::getEquipKey() {
    return equipKey;
}


/// Shop class definitions ///

void Shop::addItem(Item item) {
    items.emplace_back(item);
}

Item Shop::getItem(std::string name) {
    for (Item item : items) {
        if (item.getName() == name) {
            return item;
        }
    }

    exit(errno);
}

std::optional<Item> Shop::getItem(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        return items[index];
    }
    return std::nullopt;
}

std::optional<Item> Shop::getItemByID(int ID) {
    for (Item item : items) {
        if (item.getID() == ID) {
            return item;
        }
    }

    return std::nullopt;
}

int Shop::getTotalItems() {
    return items.size();
}

bool Shop::hasOneBg() {
    for (int i = 0; i < items.size(); i++) {
        if (ShopManager::queryPurchases(i) && items.at(i).getType() == ItemType::Background) {
            return true;
        }
    }

    return false;
}

bool Shop::retrieveItemsFromCsv(const std::string& filename) {
    return true;
}

/// Shelf class definitions ///

Shelf::Shelf(int columns, int rows, sf::Vector2f position, sf::Vector2f displaySize, sf::Vector2f gaps)
    : columns(columns), rows(rows), position(position), displaySize(displaySize), gaps(gaps)
    {
        int quantity = columns * rows;

        float additiveX = 0.f; // X axis distance from 0-indexed shelf button
        float additiveY = 0.f; // Y axis distance from 0-indexed shelf button

        itemsPulled.resize(quantity);

        passed = false;

        for (int i = 0; i < quantity; i++) {
            // Calculate initial position of new button
            additiveX = (i % columns) * (gaps.x + displaySize.x);
            additiveY = ((i / columns) % rows) * (gaps.y + displaySize.y * 1.5f + 5.f);

            // Place new button at position
            itemDisplay.emplace_back(Button(displaySize.x, displaySize.y, position.x + additiveX, position.y + additiveY, MEDIUM_BUTTON, "", "gameFont"));
            itemCaption.emplace_back(Button(displaySize.x, displaySize.y/2.f, position.x + additiveX, position.y + displaySize.y + 5.f + additiveY, MEDIUM_BUTTON, "", "gameFont"));

            itemCaption.at(i).deactivate();

            if (position.x + additiveX >= WINDOW_WIDTH && !passed) {
                passed = true;

                initScreenRightmost = itemDisplay.at(i - rows).getOriginalPos().x;
            }
        }

        globalRightmost = itemDisplay.back().getOriginalPos().x;

        float leftmost = itemDisplay.at(0).getOriginalPos().x;
        float bottom = itemCaption.at(quantity/2).getOriginalPos().y + displaySize.y/2;
        float sliderLength = initScreenRightmost + displaySize.x - leftmost;
        
        shopScroller = Slider(sliderLength, sf::Vector2f(leftmost, bottom+20.f), 0.f, sf::Color::Black);
        shopScroller.deactivate();
    }

void Shelf::display(sf::RenderTexture& renderTexture) {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        if (itemCaption.at(i).getString() != "") {
            itemDisplay.at(i).display(renderTexture);
            itemCaption.at(i).display(renderTexture);
        }
    }

    if (passed) {
        shopScroller.display(renderTexture);
    }
}

void Shelf::pullShop(int index) {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        auto optItem = Shop::getItem(index);

        if (!optItem) {
            break;
        } else {
            itemDisplay.at(i).setTexture(ResourceManager::getTexture(optItem->getPreviewKey()));
            itemCaption.at(i).setText(optItem->getDescription());
            itemsPulled.at(i) = *optItem;

            if (ShopManager::queryPurchases(itemsPulled.at(i).getID())) {
                itemsPulled.at(i).setPurchased();
                itemDisplay.at(i).deactivate();
                itemDisplay.at(i).setColor(itemDisplay.at(i).getTheme().hovering);
                toggleCaption(itemCaption.at(i), false);
            }

            if (ShopManager::queryEquips(itemsPulled.at(i).getID())) {
                itemsPulled.at(i).setEquip();
                toggleCaption(itemCaption.at(i), true);
            }

            index++;
        }
    }
}

void Shelf::updateHover(const sf::Vector2f& mousePos) {
    for (Button& item : itemDisplay) {
        if (item.isActive()) {
            item.updateHover(mousePos);
        }
    }

    for (Button& caption: itemCaption) {
        if (caption.isActive()) {
            caption.updateHover(mousePos);
        }
    }
}

void Shelf::activate() {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        if (!itemsPulled.at(i).isPurchased()) {
            itemDisplay.at(i).activate();
        } else {
            itemDisplay.at(i).setColor(itemDisplay.at(i).getTheme().hovering);
            itemCaption.at(i).activate();
        }
    }

    if (passed) {
        shopScroller.activate();
    }
}

void Shelf::deactivate() {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        itemDisplay.at(i).deactivate();
        itemCaption.at(i).deactivate();
    }

    shopScroller.deactivate();
}

bool Shelf::updateShelf(const sf::Vector2f& mousePos) {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        if (itemDisplay.at(i).frame.getGlobalBounds().contains(mousePos) && itemDisplay.at(i).isActive()) {
            if (itemsPulled.at(i).purchase()) {
                if (itemsPulled.at(i).getType() == ItemType::Hint) {
                    itemCaption.at(i).setText(itemsPulled.at(i).getName() + "\nCost: " + std::to_string(itemsPulled.at(i).getCost()) + "\nTotal: " + std::to_string(ShopManager::getHints()));
                } else {
                    itemDisplay.at(i).deactivate();
                    itemDisplay.at(i).setColor(itemDisplay.at(i).getTheme().hovering);
                    toggleCaption(itemCaption.at(i), true);

                    for (int j = 0; j < maximum; j++) {
                        if (itemsPulled.at(j).getType() == itemsPulled.at(i).getType() && itemsPulled.at(j).getID() != itemsPulled.at(i).getID() && itemsPulled.at(j).isPurchased()) {
                            itemsPulled.at(j).setEquip(false);
                            toggleCaption(itemCaption.at(j), false);
                        }
                    }
                }
            }
            return true;
        }

        if (itemCaption.at(i).frame.getGlobalBounds().contains(mousePos) && itemCaption.at(i).isActive()) {
            if (itemsPulled.at(i).isEquipped()) {
                itemsPulled.at(i).setEquip(false);
                toggleCaption(itemCaption.at(i), false);

                ShopManager::removeEquip(itemsPulled.at(i));
            } else {
                itemsPulled.at(i).setEquip();
                toggleCaption(itemCaption.at(i), true);

                for (int j = 0; j < maximum; j++) {
                    if (itemsPulled.at(j).getType() == itemsPulled.at(i).getType() && itemsPulled.at(j).getID() != itemsPulled.at(i).getID() && itemsPulled.at(j).isPurchased()) {
                        itemsPulled.at(j).setEquip(false);
                        toggleCaption(itemCaption.at(j), false);
                    }
                }

                ShopManager::changeEquips(itemsPulled.at(i));
            }

            ShopManager::saveInfo("shop.dat");

            return true;
        }
    }

    return false;
}

void Shelf::updateHintCount() {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        if (itemsPulled.at(i).getType() == ItemType::Hint) {
            itemCaption.at(i).setText(itemsPulled.at(i).getName() + "\nCost: " + std::to_string(itemsPulled.at(i).getCost()) + "\nTotal: " + std::to_string(ShopManager::getHints()));

            break;
        }
    }
}

void Shelf::scrollShelf(const sf::Event& event, sf::RenderWindow& window) {
    if (passed) {
        shopScroller.handleEvent(event, window);

        float xDistance = globalRightmost - initScreenRightmost;

        for (int i = 0; i < Shop::getTotalItems(); i++) {
            itemDisplay.at(i).setPosition(itemDisplay.at(i).getOriginalPos() - sf::Vector2f(xDistance * shopScroller.getPercentage()/100, 0.f));
            itemCaption.at(i).setPosition(itemCaption.at(i).getOriginalPos() - sf::Vector2f(xDistance * shopScroller.getPercentage()/100, 0.f));
        }
    }
}

void Shelf::toggleCaption(Button& caption, bool val) {
    if (val) {
        caption.activate();
        caption.setTheme(EASY_BUTTON);
        caption.setText("Equipped!");
    } else {
        caption.activate();
        caption.setTheme(HARD_BUTTON);
        caption.setText("Equip");
    }
}