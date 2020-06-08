#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>

#include "redhand/math.hpp"

namespace redhand{

/**
 * @brief This struct stores all the information needed to create a 2 dimensional image from a file.
 * 
 */
typedef struct{
    ///The name of the image
    std::string name;

    ///The location of the image file
    std::string file_location;

    ///
    int wrap_S;

    ///
    int wrap_T;

    ///
    int texture_min_filter;

    ///
    int texture_max_filter;

    ///The data type of the image
    int data_type;

    ///True if the program should create mipmaps
    bool create_mipmap;

    ///The internal data type that should be used
    int internal_data_type;

} image_properties;

const redhand::image_properties DEFAULT_IMAGE_PROPERTIES = {
    "texture",
    "",
    GL_TEXTURE_WRAP_S,
    GL_TEXTURE_WRAP_T,
    GL_LINEAR_MIPMAP_LINEAR,
    GL_LINEAR,
    GL_RGBA,
    true,
    GL_RGBA
};

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

    ///the properties of the texture
    image_properties texture_properties = DEFAULT_IMAGE_PROPERTIES;
    ///the mutex for the texture_properties
    std::shared_mutex mutex_texture_properties;

    ///initilizes the texture
    void initTexture2D();

public:
    /**
     * This constructor creates a 2-dimensional texture from a given filename.
     * @param prop The properties that should be used to create the image.
     */
    texture2D(image_properties prop);

    /**
     * @brief This constructor creates a 2-dimensional texture from a given filename and texture name.
     * All other properties are the DEFAULT_IMAGE_PROPERTIES
     * 
     * @param file_location the location of the file
     * @param texture_name the internale name of the texture
     */
    texture2D(std::string file_location, std::string texture_name);

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
     * This funtion returns the name of the object.
     */
    std::string_view getName();

};



}//end of namespace