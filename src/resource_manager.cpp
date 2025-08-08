#include <iostream>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "resource_manager.hpp"

std::map<std::string, sf::Font> ResourceManager::fonts;
std::map<std::string, sf::Texture> ResourceManager::textures;

void ResourceManager::loadFont(const std::string& name, const std::string& filename) {
    sf::Font font;
    if (!font.loadFromFile(filename)) {
        std::cerr << "Failed to load font.\n";
        exit(1);
    }
    fonts[name] = font;
}

sf::Font& ResourceManager::getFont(const std::string& name) {
    try {
        return fonts.at(name);
    } catch (const std::out_of_range& ex) {
        std::cerr << "out_of_range::what(): " << ex.what() << '\n';
        exit(1);
    }
}

void ResourceManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Image image;
    if (!image.loadFromFile(filename)) {
        std::cerr << "Failed to load image.\n";
        exit(1);
    }

    sf::Texture texture;
    texture.loadFromImage(image);

    textures[name] = texture;
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    try {
        return textures.at(name);
    } catch (const std::out_of_range& ex) {
        std::cerr << "out_of_range::what(): " << ex.what() << '\n';
        exit(1);
    }
}
