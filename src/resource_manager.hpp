#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

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
     * @throw std::out_of_range
    */
    static sf::Font& getFont(const std::string& name);

    /**
     * @return The texture in the textures map with the corresponding key
     * @param name The key of the desired texture
     * @throw std::out_of_range
    */
    static sf::Texture& getTexture(const std::string& name);

    /**
     * @return The audio file in the audios map with the corresponding key
     * @param name The key of the desired audio
     * @throw std::out_of_range
    */
    static sf::Music& getAudio(const std::string& name);

    /**
     * @return The shader file in the shaders map with the corresponding key
     * @param name The key of the desired shader 
     * @throw std::out_of_range
    */
    static sf::Shader& getShader(const std::string& name);

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

    /**
     * Adds an audio and its key to the audios map
     * @param name The new audio's key for the map
     * @param filename The path to the audio file being used
    */
    static void loadAudio(const std::string& name, const std::string& filename);

    /**
     * Adds a shader and its key to the shaders map
     * @param name The new audio's key for the map
     * @param filename The path to the audio file being used
    */
    static void loadShader(const std::string& name, const std::string& filename);

private:

    //Map containing all loaded fonts
    static std::map<std::string, sf::Font> fonts;

    //Map containing all loaded textures
    static std::map<std::string, sf::Texture> textures;

    //Map containing all loaded audios
    static std::map<std::string, std::unique_ptr<sf::Music>> audios;

    //Map containing all loaded shaders
    static std::map<std::string, std::unique_ptr<sf::Shader>> shaders;
};

#endif