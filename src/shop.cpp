#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <SFML/Graphics.hpp>

#include "shop.hpp"
#include "record.hpp"
#include "sudoku.hpp"
#include "button.hpp"
#include "resource_manager.hpp"
#include "shop_manager.hpp"

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

Item::Item(int cost, std::string name, std::string texture, ItemType type)
    : cost(cost), name(name), type(type)
    {
        purchased = false;
        equipped = false;
        itemID = counter++;
        previewKey = texture;

        description = name + "\nCost: " + std::to_string(cost);
    }

int Item::getID() {
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

bool Item::isPurchased() {
    return purchased;
}

void Item::setPurchased() {
    purchased = true;
}

void Item::purchase() {
    if (cost <= SaveManager::getPoints() && !purchased) {
        SaveManager::addPoints(-cost);
        SaveManager::saveRecords("records.dat");
        purchased = true;
        ShopManager::saveShop("shop.dat", itemID);
    }
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

int Shop::getSize() {
    return items.size();
}


/// Shelf class definitions ///

Shelf::Shelf(int columns, int rows, sf::Vector2f position, sf::Vector2f displaySize, sf::Vector2f gaps)
    : columns(columns), rows(rows), position(position), displaySize(displaySize), gaps(gaps)
    {
        int quantity = columns * rows;

        float additiveX = 0.f;
        float additiveY = 0.f;

        itemsPulled.resize(quantity);

        for (int i = 0; i < quantity; i++) {
            additiveX = (i % columns) * (gaps.x + displaySize.x);
            additiveY = ((i / columns) % rows) * (gaps.y + displaySize.y * 1.5f + 5.f);

            itemDisplay.emplace_back(Button(displaySize.x, displaySize.y, position.x + additiveX, position.y + additiveY, MEDIUM_BUTTON, "", "gameFont"));
            itemCaption.emplace_back(Button(displaySize.x, displaySize.y/2.f, position.x + additiveX, position.y + displaySize.y + 5.f + additiveY, MEDIUM_BUTTON, "", "gameFont"));

            itemCaption.at(i).deactivate();
        }
    }

void Shelf::display(sf::RenderTexture& renderTexture) {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        if (itemCaption.at(i).getString() != "") {
            itemDisplay.at(i).display(renderTexture);
            itemCaption.at(i).display(renderTexture);
        }
    }
}

void Shelf::pullShop(int index) {
    int maximum = columns * rows;
    sections = Shop::getSize() / maximum + 1;

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
                itemDisplay.at(i).frame.setFillColor(itemDisplay.at(i).getTheme().hovering);
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
}

void Shelf::activate() {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        if (!itemsPulled.at(i).isPurchased()) {
            itemDisplay.at(i).activate();
        }
    }
}

void Shelf::deactivate() {
    for (Button& item : itemDisplay) {
        item.deactivate();
    }
}

void Shelf::updateShelf(const sf::Vector2f& mousePos) {
    int maximum = columns * rows;

    for (int i = 0; i < maximum; i++) {
        if (itemDisplay.at(i).frame.getGlobalBounds().contains(mousePos) && itemDisplay.at(i).isActive()) {
            itemsPulled.at(i).purchase();
        }
    }
}