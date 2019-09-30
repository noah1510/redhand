#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class texture2D{
private:
    bool errord = false;
    unsigned int id;
    int width;
    int height;
public:
    texture2D(const char* filename);

    bool hasErrord();
    unsigned int getID();
    void bind(int unit);
};



