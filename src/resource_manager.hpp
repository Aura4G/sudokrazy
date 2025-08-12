#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

/**
 * Resource Management functions
 * @author Aura4G (Aria Noroozi)
*/

//static class to manage all external resources like images and fonts
class ResourceManager {
public:

    /**
     * @return The font file in the fonts map with the corresponding key
     * @param name The key of the desired font
    */
    static sf::Font& getFont(const std::string& name);

    /**
     * @return The texture in the textures map with the corresponding key
     * @param name The key of the desired texture
    */
    static sf::Texture& getTexture(const std::string& name);

    /**
     * Adds a font and its key to the fonts map
     * @param name The new font's key for the map
     * @param filename The path to the .otf/.ttf file being used
    */
    static void loadFont(const std::string& name, const std::string& filename);

    /**
     * Adds a texture and its key to the textures map
     * @param name The new texture's key for the map
     * @param filename The path to the image file being used
    */
    static void loadTexture(const std::string& name, const std::string& filename);

private:

    //Map containing all loaded fonts
    static std::map<std::string, sf::Font> fonts;

    //Map containing all loaded textures
    static std::map<std::string, sf::Texture> textures;
};

#endif