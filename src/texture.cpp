#include "redhand/texture.hpp"

using namespace redhand;

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"

void redhand::initSTB(){
    stbi_set_flip_vertically_on_load(true); 
}

void redhand::texture2D::initTexture2D(){
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_properties.wrap_S);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_properties.wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_properties.texture_min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_properties.texture_max_filter);

    // load and generate the texture
    int nrChannels;
    unsigned char *data = stbi_load(texture_properties.file_location.c_str(), &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, texture_properties.internal_data_type, width, height, 0, texture_properties.data_type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
        errord = true;
    }
    stbi_image_free(data);
}

///@todo add file checks
redhand::texture2D::texture2D(image_properties prop){

    texture_properties = prop;

    initTexture2D();
}

redhand::texture2D::texture2D(std::string file_location, std::string texture_name){

    texture_properties.name = texture_name;
    texture_properties.file_location = file_location;

    initTexture2D();
}

bool redhand::texture2D::hasErrord(){
    auto lock = std::shared_lock(mutex_errored);
    return errord;
}

unsigned int redhand::texture2D::getID(){
    auto lock = std::shared_lock(mutex_id);
    return id;
}

void redhand::texture2D::bind(int unit){
    auto lock = std::shared_lock(mutex_id);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

int redhand::texture2D::getWidth(){
    auto lock = std::shared_lock(mutex_width);
    return width;
}

int redhand::texture2D::getHeight(){
    auto lock = std::shared_lock(mutex_height);
    return height;
}

std::string_view redhand::texture2D::getName(){
    auto lock = std::shared_lock(mutex_texture_properties);
    return texture_properties.name;
}
