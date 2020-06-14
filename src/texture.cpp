#include <redhand/glad/glad.h>
#include "redhand/texture.hpp"

using namespace redhand;

void redhand::initImageLoader(){
    Magick::InitializeMagick(nullptr);
}

void redhand::texture2D::initTexture2D(){

    //generate (if needed) and bind the texture
    if(id == 0){glGenTextures(1, &id);}
    glBindTexture(GL_TEXTURE_2D, id);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_properties.wrap_S);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_properties.wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_properties.texture_min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_properties.texture_max_filter);

    //get the image if it is only temporary
    if(!texture_properties.keep_image_data){
        image_data = std::unique_ptr<Magick::Image>(new Magick::Image(texture_properties.file_location));
    }

    //Format data
    image_data->flip();
    image_data->depth(8);
    image_data->magick("RGBA");

    //copy image to blob
    Magick::Blob data;
    image_data->write(&data, "RGBA");

    //update the dimensions
    width = image_data->columns();
    height = image_data->rows();

    //create opengl texture
    if (data.length() != 0){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
        errord = true;
    }

    //If the image should not be kept delete it
    if(!texture_properties.keep_image_data){
        image_data.release();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

///@todo add file checks
redhand::texture2D::texture2D(image_properties prop){
    texture_properties = prop;

    if(texture_properties.file_location.compare("") == 0){
        return;
    }

    if(texture_properties.keep_image_data){
        image_data = std::unique_ptr<Magick::Image>(new Magick::Image(texture_properties.file_location));
    }

    initTexture2D();
}

redhand::texture2D::texture2D(std::string file_location, std::string texture_name){
    texture_properties.file_location = std::filesystem::current_path().append(file_location);
    //texture_properties.file_location = file_location;
    texture_properties.name = texture_name;

    if(texture_properties.file_location.compare("") == 0){
        return;
    }

    if(texture_properties.keep_image_data){
        image_data = std::unique_ptr<Magick::Image>(new Magick::Image(texture_properties.file_location));
    }

    initTexture2D();
}

redhand::texture2D::~texture2D(){
    image_data.release();
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
