#include "redhand/texture.hpp"

using namespace redhand;

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"

void redhand::initSTB(){
    stbi_set_flip_vertically_on_load(true); 
}

redhand::texture2D::texture2D(const char* filename, int Wrap_S, int Wrap_T){
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load and generate the texture
    int nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
        errord = true;
    }
    stbi_image_free(data);
}

redhand::texture2D::texture2D(
    const char* filename
):texture2D(
    filename,
    GL_REPEAT,
    GL_REPEAT
){}

redhand::texture2D::texture2D(
    const char* filename, 
    std::string name
):texture2D(
    filename,
    GL_REPEAT,
    GL_REPEAT
){
    texture_name = name;
}

redhand::texture2D::texture2D(
    const char* filename,
    int Wrap_S, 
    int Wrap_T, 
    std::string name
):texture2D(
    filename,
    Wrap_S,
    Wrap_T
){
    texture_name = name;
}

bool redhand::texture2D::hasErrord(){
    auto lock = std::shared_lock(mutex_errored);
    return errord;
}

unsigned int redhand::texture2D::getID(){
    auto lock = std::shared_lock(mutex_id);
    return id;
}

void redhand::texture2D::bind(int unit = 0){
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

void redhand::texture2D::setName(std::string name){
    auto lock = std::scoped_lock(mutex_texture_name);
    texture_name = name;
}
std::string redhand::texture2D::getName(){
    auto lock = std::shared_lock(mutex_texture_name);
    return texture_name;
}
