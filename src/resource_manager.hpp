#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
public:
    static sf::Font& getFont(const std::string& name);
    static sf::Texture& getTexture(const std::string& name);
    static void loadFont(const std::string& name, const std::string& filename);
    static void loadTexture(const std::string& name, const std::string& filename);

private:
    static std::map<std::string, sf::Font> fonts;
    static std::map<std::string, sf::Texture> textures;
};

#endif