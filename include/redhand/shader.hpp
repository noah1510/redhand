#pragma once

#include <redhand/glad/glad.h> // include glad to get all the required OpenGL headers
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>
#include <shared_mutex>

#include "redhand/math.hpp"
#include "redhand/shaderSource.hpp"

namespace redhand{

class shader{
private:
    /// the program ID
    unsigned int ID;

    ///the name of the shader
    std::string shader_name = "shader";
    ///the mutex for the shader_name
    std::shared_mutex mutex_shader_name;

    ///true if an error happened
    bool errord = false;
    ///the mutex for the errord
    std::shared_mutex mutex_errord;

    ///true if shader has been fully constructed
    bool initilized = false;
    ///the mutex for the initilized
    std::shared_mutex mutex_initilized;

    ///The position of the camera
    glm::vec3 cameraVector = glm::vec3(0.0f, 0.0f, 0.0f);
    ///the mutex for the cameraVector
    std::shared_mutex mutex_cameraVector;

    ///The camera matrix
    glm::mat4 camera = glm::mat4(1.0f);
    ///the mutex for the camera
    std::shared_mutex mutex_camera;

    ///the projection matrix
    glm::mat4 projectionMatrix;
    ///the mutex for the projectionMatrix
    std::shared_mutex mutex_projectionMatrix;

    ///the texture scale
    glm::vec2 textureScale = glm::vec2(1.0f, 1.0f);
    ///the mutex for the texture scale
    std::shared_mutex mutex_textureScale;

    ///This function basically just reads a file into a string
    std::string loadShaderCodeFromFile(std::string_view fileLocation);

    ///This function compiles a given shader source code and returns the id of the compiled shader or 0 if there was an error.
    unsigned int compileShader(std::string_view shaderCode, int shaderType);

    ///This function links a vertex and fragment shader and if no error happened it will set initilized to true
    void linkShader(unsigned int vertexShader, unsigned int fragmentShader);

public: 
    /** 
     * This constructor creates a shader with the default vector and fragment shaders named "default".
     */
    shader();

    /**
     * @brief Destroy the shader object and delete the shader program
     * 
     */
    ~shader();

    /**
     * This constructor creates a shader with a given name.
     * The default vertex and fragment shaders from the shaderSource.hpp will be used.
     * @param name The name of the Shader
     */
    shader(std::string_view name);

    /**
     * @brief Create a Shader From a file.
     * 
     * @param vertexPath The location of the vertex Shader
     * @param fragmentPath The location of the Fragment Shader
     * @return int negative if something went wrong
     */
    int createShaderFromFile(std::string_view vertexPath, std::string_view fragmentPath);

    /**
     * @brief Create a Shader From source Code
     * @warning make sure the location the string_view sees still has the string while the function is executed.
     * 
     * @param vertexSource The source code of the vertex shader
     * @param fragmentSource The source code of the fragment shader
     * @return int 
     */
    int createShaderFromCode(std::string_view vertexSource, std::string_view fragmentSource);

    /**
     * @brief Initilizes the shader object with the default shader.
     * 
     * @return int negative value if anything went wrong
     */
    int createDefaultShader();

    /**
     * This function returns true if an error happend during the setup of the shader or the shader is not initilized.
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
    std::string_view getName();

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
    void setCamera(float pos_x, float pos_y);

    /**
     * This function is used to set the position of the camera.
     * All units are in world space cooridinates
     * @param delta_pos_x The amount the camera should move along the x-Axis
     * @param delta_pos_y The amount the camera should move along the y-Axis
     */
    void moveCamera(float delta_pos_x, float delta_pos_y);

    /**
     * This function sets the projection matrix of the shader to the given matrix.
     * 
     * @param projection the projection matrix which should be used
     */
    void setProjectionmatrix(glm::mat4 projection);

    /**
     * This function sets the texture scale of the shader to the given vector.
     * 
     * @param scale the texture scale which should be used
     */
    void setTextureScale(glm::vec2 scale);
};

}//end of namespace