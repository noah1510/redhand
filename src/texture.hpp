#pragma once
#include <string>
#include <glad/glad.h>
#include <gitGLFW/glfw3.h>

#include <iostream>

void initSTB();

class texture2D{
private:
    bool errord = false;
    unsigned int id;
    int width;
    int height;
public:
    texture2D(const char* filename);
    texture2D(const char* filename, int Wrap_S, int Wrap_T);

    bool hasErrord();
    unsigned int getID();
    void bind(int unit);
};



