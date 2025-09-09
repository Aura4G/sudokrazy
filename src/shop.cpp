#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "shop.hpp"
#include "record.hpp"
#include "sudoku.hpp"
#include "button.hpp"

int Item::counter = 0;

std::vector<Item> Shop::items;

Item::Item(int cost, std::string& name, std::string& description)
    : cost(cost), name(name), description(description)
    {
        purchased = false;
        itemID = counter++;
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

bool Item::isPurchased() {
    return purchased;
}

void Item::purchase() {
    if (cost <= SaveManager::getPoints() && !purchased) {
        SaveManager::addPoints(-cost);
        purchased = true;
    }
}


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

Shelf::Shelf(int columns, int rows, sf::Vector2f position, sf::Vector2f displaySize, sf::Vector2f gaps)
    : columns(columns), rows(rows), position(position), displaySize(displaySize), gaps(gaps)
    {
        int quantity = columns * rows;

        float additiveX = 0.f;
        float additiveY = 0.f;

        for (int i = 0; i < quantity; i++) {
            additiveX = (i % columns) * (gaps.x + displaySize.x);
            additiveY = (i % rows) * (gaps.y + displaySize.y * 1.5f + 5.f);

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