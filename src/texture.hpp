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

public:
    texture2D(const char* filename);
    texture2D(const char* filename, int Wrap_S, int Wrap_T);

    bool hasErrord();
    unsigned int getID();
    void bind(int unit);

    int getWidth();
    int getHeight();
};



