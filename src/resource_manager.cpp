#include <iostream>
#include <string>
#include <map>
#include <cerrno>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "resource_manager.hpp"

std::map<std::string, sf::Font> ResourceManager::fonts;
std::map<std::string, sf::Texture> ResourceManager::textures;
std::map<std::string, std::unique_ptr<sf::Music>> ResourceManager::audios;
std::map<std::string, std::unique_ptr<sf::Shader>> ResourceManager::shaders;

void ResourceManager::loadFont(const std::string& name, const std::string& filename) {
    sf::Font font;
    if (!font.loadFromFile(filename)) {
        std::cerr << "Failed to load font.\n";
        exit(errno);
    }
    fonts[name] = font;
}

sf::Font& ResourceManager::getFont(const std::string& name) {
    try {
        return fonts.at(name);
    } catch (const std::out_of_range& ex) {
        std::cerr << "out_of_range::what(): " << ex.what() << '\n';
        exit(errno);
    }
}

void ResourceManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Image image;
    if (!image.loadFromFile(filename)) {
        std::cerr << "Failed to load image.\n";
        exit(errno);
    }

    //The image object is loaded into a texture, that's then stored in the map
    sf::Texture texture;
    texture.loadFromImage(image);

    textures[name] = texture;
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    try {
        return textures.at(name);
    } catch (const std::out_of_range& ex) {
        std::cerr << "out_of_range::what(): " << ex.what() << '\n';
        exit(errno);
    }
}

void ResourceManager::loadAudio(const std::string& name, const std::string& filename) {
    //Music object duplication is a deleted function, so we have to use unique pointers and memory movement for music
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filename)) {
        std::cerr << "Error finding music file: " << filename << std::endl;
        exit(errno);
    }

    audios[name] = std::move(music);
}

sf::Music& ResourceManager::getAudio(const std::string& name) {
    try {
        return *audios.at(name);
    } catch (const std::out_of_range& ex) {
        std::cerr << "out_of_range::what(): " << ex.what() << '\n';
        exit(errno);
    }
}

void ResourceManager::loadShader(const std::string& name, const std::string& filename) {
    //Shader object duplication is a deleted function, so we have to use unique pointers and memory movement for shader
    auto shader = std::make_unique<sf::Shader>();
    if (!shader->loadFromFile(filename, sf::Shader::Fragment)) {
        std::cerr << "Error finding shader file: " << filename << std::endl;
        exit(errno);
    }

    shaders[name] = std::move(shader);
}

sf::Shader& ResourceManager::getShader(const std::string& name) {
    try {
        return *shaders.at(name);
    } catch (const std::out_of_range& ex) {
        std::cerr << "out_of_range::what(): " << ex.what() << '\n';
        exit(errno);
    }
}