#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    //string if an error happened
    bool errord = false;

    //These Variables hold the position of the camera
    glm::vec3 cameraVector = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 camera = glm::mat4(1.0f);

public: 
    /** 
     * This constructor creates a shader with the default vector and fragment shaders named "default".
     */
    shader();

    /**
     * This constructor creates a shader from a given vertex and fragment shader.
     * The given paths are relative to the executeable.
     * @param vertexPath The Path to the vertex Shader
     * @param fragmentPath The Path to the fragment Shader
     */
    shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    /**
     * This constructor creates a shader from a given vertex and fragment shader and sets a given name.
     * The given paths are relative to the executeable.
     * @param vertexPath The Path to the vertex Shader
     * @param fragmentPath The Path to the fragment Shader
     * @param name The name of the Shader
     */
    shader(const GLchar* vertexPath, const GLchar* fragmentPath, std::string name);

    /**
     * This function returns true if an error happend during the setup of the shader.
     */
    bool hasErrored();

    /**
     * This function activates the shader and sets the camera position.
     */
    void use();

    /**
     * This function return the name the shader has.
     * It will be "shader" if nothing else was specified.
     */
    std::string getName();

    /**
     * This function returns the ID of the shader for the use in some OpenGL functions.
     */
    unsigned int getID();

    // utility uniform functions
    /**
     * The setBool functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     */
    void setBool(const std::string &name, bool value) const;

    /**
     * The setBool functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     */
    void setBool(const std::string &name, bool value, bool value2) const;

    /**
     * The setBool functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     * @param value3 The z component of the Vector
     */
    void setBool(const std::string &name, bool value, bool value2, bool value3) const;

    /**
     * The setBool functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     * @param value3 The z component of the Vector
     * @param value4 The w component of the Vector
     */
    void setBool(const std::string &name, bool value, bool value2, bool value3, bool value4) const;

    /**
     * The setInt functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     */
    void setInt(const std::string &name, int value) const;

    /**
     * The setInt functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     */
    void setInt(const std::string &name, int value, int value2) const;

    /**
     * The setInt functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     * @param value3 The z component of the Vector
     */
    void setInt(const std::string &name, int value, int value2, int value3) const;

    /**
     * The setInt functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     * @param value3 The z component of the Vector
     * @param value4 The w component of the Vector
     */
    void setInt(const std::string &name, int value, int value2, int value3, int value4) const;

    /**
     * The setFloat functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     */
    void setFloat(const std::string &name, float value) const;

    /**
     * The setFloat functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     */
    void setFloat(const std::string &name, float value, float value2) const;

    /**
     * The setFloat functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     * @param value3 The z component of the Vector
     */
    void setFloat(const std::string &name, float value, float value2, float value3) const;

    /**
     * The setFloat functions set a uniform vector of the shader to given values
     * @param name The name of the uniform variable
     * @param value The x component of the Vector
     * @param value2 The y component of the Vector
     * @param value3 The z component of the Vector
     * @param value4 The w component of the Vector
     */
    void setFloat(const std::string &name, float value, float value2, float value3, float value4) const;
    
    /**
     * This function gets a uniform float from an uniform vector and saves it to the destination.
     * @param name The name of the uniform variable
     * @param dest The array where the contents of the variables will be saved to
     */
    void getFloat(const std::string &name, float dest[]) const;

    /**
     * This function is used to set the position of the camera.
     * All units are in world space cooridinates.
     * @param pos_x The x coordinate of the camera
     * @param pos_y The y coordinate of the camera
     */
    void setCamera(float pos_x, float pox_y);

    /**
     * This function is used to set the position of the camera.
     * All units are in world space cooridinates
     * @param delta_pos_x The amount the camera should move along the x-Axis
     * @param delta_pos_y The amount the camera should move along the y-Axis
     */
    void moveCamera(float delta_pos_x, float delta_pox_y);
};

#endif