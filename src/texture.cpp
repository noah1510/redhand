#include <redhand/glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "redhand/texture.hpp"

using namespace redhand;

void redhand::initImageLoader(char *str) {
    stbi_set_flip_vertically_on_load(true); 
}

void redhand::texture2D::loadImage(){
    std::scoped_lock<std::shared_mutex,std::shared_mutex,std::shared_mutex,std::shared_mutex> lock(mutex_height,mutex_width,mutex_channels,mutex_image_data);
    image_data = stbi_load(texture_properties.file_location.string().c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
}

void redhand::texture2D::freeImage() {
    std::scoped_lock<std::shared_mutex,std::shared_mutex,std::shared_mutex,std::shared_mutex> lock(mutex_height,mutex_width,mutex_channels,mutex_image_data);
    stbi_image_free(image_data);
    width = 0;
    height = 0;
    nrChannels = 0;
}

void redhand::texture2D::initTexture2D() {

    //generate (if needed) and bind the texture
    if (id == 0) {
        glGenTextures(1, &id);
    }
    glBindTexture(GL_TEXTURE_2D, id);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_properties.wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_properties.wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_properties.texture_min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_properties.texture_max_filter);

    //get the image if it is only temporary
    
    if (!texture_properties.keep_image_data) {
        loadImage();
    }

    //create opengl texture
    if (image_data != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        errord = true;
    }

    //If the image should not be kept delete it
    if (!texture_properties.keep_image_data) {
        freeImage();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

///@todo add file checks
redhand::texture2D::texture2D(image_properties prop) {
    texture_properties = prop;

    if (texture_properties.file_location.compare("") == 0) {
        return;
    }

    if (texture_properties.keep_image_data) {
       loadImage();
    }

    initTexture2D();
}

redhand::texture2D::texture2D(std::string file_location, std::string texture_name) {
    texture_properties.file_location = std::filesystem::current_path().append(file_location);
    //texture_properties.file_location = file_location;
    texture_properties.name = texture_name;

    if (texture_properties.file_location.compare("") == 0) {
        return;
    }

    if (texture_properties.keep_image_data) {
        loadImage();
    }

    initTexture2D();
}

redhand::texture2D::~texture2D() {
    freeImage();
    if(id != 0){
        glDeleteTextures(1,&id);
    }
}

bool redhand::texture2D::hasErrord() {
    auto lock = std::shared_lock(mutex_errored);
    return errord;
}

unsigned int redhand::texture2D::getID() {
    auto lock = std::shared_lock(mutex_id);
    return id;
}

void redhand::texture2D::bind(int unit) {
    auto lock = std::shared_lock(mutex_id);
    int max_units = 0;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&max_units);
    unit %= max_units;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

int redhand::texture2D::getWidth() {
    auto lock = std::shared_lock(mutex_width);
    return width;
}

int redhand::texture2D::getHeight() {
    auto lock = std::shared_lock(mutex_height);
    return height;
}

std::string_view redhand::texture2D::getName() {
    auto lock = std::shared_lock(mutex_texture_properties);
    return texture_properties.name;
}

image_properties redhand::texture2D::getProperties(){
    auto lock = std::shared_lock(mutex_texture_properties);
    return texture_properties;
}
