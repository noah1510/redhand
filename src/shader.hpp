#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <gitGLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class shader{
private:
    // the program ID
    unsigned int ID;

    //the name of the shader
    std::string shader_name = "shader";

    //storing if an error happened
    bool errord = false;
public: 

    // constructor reads and builds the shader
    shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    // constructor reads and builds the shader and sets a name
    shader(const GLchar* vertexPath, const GLchar* fragmentPath, std::string name);

    //see if there was an error
    bool hasErrored();

    // use/activate the shader
    void use();

    //get the name of the shader
    std::string getName();

    //get the ID of the shader
    unsigned int getID();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setBool(const std::string &name, bool value, bool value2) const;
    void setBool(const std::string &name, bool value, bool value2, bool value3) const;
    void setBool(const std::string &name, bool value, bool value2, bool value3, bool value4) const;

    void setInt(const std::string &name, int value) const;
    void setInt(const std::string &name, int value, int value2) const;
    void setInt(const std::string &name, int value, int value2, int value3) const;
    void setInt(const std::string &name, int value, int value2, int value3, int value4) const;

    void setFloat(const std::string &name, float value) const;
    void setFloat(const std::string &name, float value, float value2) const;
    void setFloat(const std::string &name, float value, float value2, float value3) const;
    void setFloat(const std::string &name, float value, float value2, float value3, float value4) const;
    
    void getFloat(const std::string &name, float dest[]) const;


};

#endif