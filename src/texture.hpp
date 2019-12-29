#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void initSTB();

class texture2D{
private:
    ///true if an error happened
    bool errord = false;

    ///the id of the texture
    unsigned int id;

    //the width of the texture
    int width;

    ///the hight of the texture
    int height;

    ///the texture scale in the x direction
    float scale_x = 1.0f;

    ///the texture scale in the y direction
    float scale_y = 1.0f;

    ///the name of the texture
    std::string texture_name = "texture";

public:
    texture2D(const char* filename);
    texture2D(const char* filename, std::string name);
    texture2D(const char* filename, int Wrap_S, int Wrap_T);
    texture2D(const char* filename, int Wrap_S, int Wrap_T, std::string name);

    ///This function returns true if an error has happened
    bool hasErrord();

    ///This function returns the ID of the texture so that it can be activated by shaders
    unsigned int getID();

    ///This function binds the texture to the specified texture unit
    ///@param unit The index of the texture unit the texture will be bound to (0 by default)
    void bind(int unit);

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
    std::string getName();
};



