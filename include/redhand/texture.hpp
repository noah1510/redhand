#pragma once

#include <redhand/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>

#include "redhand/math.hpp"

namespace redhand{

void initSTB();

class texture2D{
private:
    ///true if an error happened
    bool errord = false;
    ///the mutex for the errord
    std::shared_mutex mutex_errored;

    ///the id of the texture
    unsigned int id;
    ///the mutex for the id
    std::shared_mutex mutex_id;

    ///the width of the texture
    int width;
    ///the mutex for the width
    std::shared_mutex mutex_width;

    ///the hight of the texture
    int height;
    ///the mutex for the height
    std::shared_mutex mutex_height;

    ///the name of the texture
    std::string texture_name = "texture";
    ///the mutex for the texture_name
    std::shared_mutex mutex_texture_name;

public:
    /**
     * This constructor creates a 2-dimensional texture from a given filename.
     * @param filename The name of the file to be used.
     */
    texture2D(const char* filename);

    /**
     * This constructor creates a 2-dimensional texture from a given filename.
     * @param filename The name of the file to be used.
     * @param name The name of the texture object
     */
    texture2D(const char* filename, std::string name);

    /**
     * This constructor creates a 2-dimensional texture from a given filename.
     * @param filename The name of the file to be used.
     * @param Wrap_S The texture wrapping method in x direction
     * @param Wrap_T The texture wrapping method in y direction
     */
    texture2D(const char* filename, int Wrap_S, int Wrap_T);

    /**
     * This constructor creates a 2-dimensional texture from a given filename.
     * @param filename The name of the file to be used.
     * @param Wrap_S The texture wrapping method in x direction
     * @param Wrap_T The texture wrapping method in y direction
     * @param name The name of the texture object
     */
    texture2D(const char* filename, int Wrap_S, int Wrap_T, std::string name);

    ///This function returns true if an error has happened
    bool hasErrord();

    ///This function returns the ID of the texture so that it can be activated by shaders
    unsigned int getID();

    ///This function binds the texture to the specified texture unit
    ///@param unit The index of the texture unit the texture will be bound to (0 by default)
    void bind(int unit = 0);

    /**
     * This function retruns the width of the texture
     */
    int getWidth();

    /**
     * This function retruns the height of the texture
     */
    int getHeight();

    /**
     * This function sets the name of the game_object to the given string.
     */
    void setName(std::string name);

    /**
     * This funtion returns the name of the object.
     */
    std::string_view getName();

};



}//end of namespace