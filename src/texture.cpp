#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void initSTB(){
    stbi_set_flip_vertically_on_load(true); 
}

texture2D::texture2D(const char* filename, int Wrap_S, int Wrap_T){
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

texture2D::texture2D(
    const char* filename
):texture2D(
    filename,
    GL_REPEAT,
    GL_REPEAT
){}

bool texture2D::hasErrord(){
    return errord;
}

unsigned int texture2D::getID(){
    return id;
}

void texture2D::bind(int unit){
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

int texture2D::getWidth(){
    return width;
}

int texture2D::getHeight(){
    return height;
}
