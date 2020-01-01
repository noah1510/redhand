#pragma once
#include "shader.hpp"
#include "texture.hpp"
#include "math.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @brief The game_object is a simple object which can be displayed in a world.
 * It is the first abstraction layer and is a very low level openGL object.
 * You can create a game_object by specifying all the points, and which points form a triangel.
 * Each game_object NEEDS a shader in order to work.
 * Please use a shader which is added to the same world as the game_object in order to prevent errors.
 * 
 * The Points specified are all local coordinates.
 * This means they are in the first sector of a 2 dimensional coordinate system.
 * \todo If there is any point outside of this range the creation of the object will fail.
 * For each point you must also specify the color of that point.
 * 
 * Each game_object has a name which is "game_object" by default.
 * 
 * In addition to that the object also holds its position, rotation and scale in world coordinates.
 */
class game_object{
private:

    /// The identifier of the Video Array Object
    unsigned int VAO;

    ///The identifier of the Video Buffer Object
    unsigned int VBO;

    ///The identifier of the Element Buffer Object
    unsigned int EBO;

    /// This is 3 * "number of tirangle" and is important to actually draw those triangles
    int indices_size;

    ///The attaced shader
    shader* object_shader;

    ///The attached texture
    texture2D* object_texture;

    ///The texture mode of the object
    ///
    unsigned int textureMode = 0;

    ///The alpha value of the color
    float colorAlphaValue = 1.0f;

    ///false if no error happened
    bool errored = false;
    
    ///A custom function which is called just after the shader is activated
    std::function<void(shader*)> shader_routine;

    ///A function that will be called once every tick
    std::function<void(GLFWwindow* window, game_object* obj)> LoopFunction;

    ///The position of the object in World Coordinates
    std::vector<float> object_position = {0.0f, 0.0f};

    ///The scale of the object in World scale
    std::vector<float> object_scale = {1.0f, 1.0f};

    ///The rotation of the object in degrees
    float object_rotation = 0.0f;

    ///the scale of the texture attached to this object
    glm::vec2 texture_scale = glm::vec2(1.0f, 1.0f);

    ///The name of the object
    std::string object_name = "game_object";

public:
    //minimal constructor
    /**
     * @brief Construct a new game object object
     * 
     * @param points An array with all the points this object has in local coordinates
     * @param indices The indicies which specifiy, which points form a triangle
     * @param colors The color of the points
     * @param attached_shader A pointer to the attached shader \note the shader should be added to the same world
     * @param gl_drawing_mode The drawing mode of the game_object
     */
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode);

    //minimal with name
    /**
     * @brief Construct a new game object object
     * 
     * @param points An array with all the points this object has in local coordinates
     * @param indices The indicies which specifiy, which points form a triangle
     * @param colors The color of the points
     * @param attached_shader A pointer to the attached shader \note the shader should be added to the same world
     * @param gl_drawing_mode The drawing mode of the game_object
     */
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,
        std::string name);

    //minimal with shader routine
    /**
     * @brief Construct a new game object object
     * 
     * @param points An array with all the points this object has in local coordinates
     * @param indices The indicies which specifiy, which points form a triangle
     * @param colors The color of the points
     * @param attached_shader A pointer to the attached shader \note the shader should be added to the same world
     * @param gl_drawing_mode The drawing mode of the game_object
     * @param routine The shader routine which should be used
     */
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::function<void(shader*)> routine);

    //minimal with scale,rotation and position
    /**
     * @brief Construct a new game object object
     * 
     * @param points An array with all the points this object has in local coordinates
     * @param indices The indicies which specifiy, which points form a triangle
     * @param colors The color of the points
     * @param attached_shader A pointer to the attached shader \note the shader should be added to the same world
     * @param gl_drawing_mode The drawing mode of the game_object
     * @param scaler The scale of the object in world coordinates
     * @param rotator The rotation of the object in world coordinates
     * @param postitions The position of the object in world coordinates
     */
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::vector<float> scaler,
        float rotator,
        std::vector<float> postitions);

    //minimal with scale,rotation, position and shader routine
    /**
     * @brief Construct a new game object object
     * 
     * @param points An array with all the points this object has in local coordinates
     * @param indices The indicies which specifiy, which points form a triangle
     * @param colors The color of the points
     * @param attached_shader A pointer to the attached shader \note the shader should be added to the same world
     * @param gl_drawing_mode The drawing mode of the game_object
     * @param routine The shader routine which should be used
     * @param scaler The scale of the object in world coordinates
     * @param rotator The rotation of the object in world coordinates
     * @param postitions The position of the object in world coordinates
     */
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::function<void(shader*)> routine,

        std::vector<float> scaler,
        float rotator,
        std::vector<float> postitions);

    //full constructor without texels
    /**
     * @brief Construct a new game object object
     * 
     * @param points An array with all the points this object has in local coordinates
     * @param indices The indicies which specifiy, which points form a triangle
     * @param colors The color of the points
     * @param attached_shader A pointer to the attached shader \note the shader should be added to the same world
     * @param gl_drawing_mode The drawing mode of the game_object
     * @param routine The shader routine which should be used
     * @param scaler The scale of the object in world coordinates
     * @param rotator The rotation of the object in world coordinates
     * @param postitions The position of the object in world coordinates
     * @param texture A pointer to the attaches texture
     */
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::function<void(shader*)> routine,

        std::vector<float> scaler,
        float rotator,
        std::vector<float> postitions,

        texture2D* texture);

    //full constructor
    /**
     * @brief Construct a new game object object
     * 
     * @param points An array with all the points this object has in local coordinates
     * @param indices The indicies which specifiy, which points form a triangle
     * @param colors The color of the points
     * @param attached_shader A pointer to the attached shader \note the shader should be added to the same world
     * @param gl_drawing_mode The drawing mode of the game_object
     * @param routine The shader routine which should be used
     * @param scaler The scale of the object in world coordinates
     * @param rotator The rotation of the object in world coordinates
     * @param postitions The position of the object in world coordinates
     * @param texture A pointer to the attaches texture
     * @param texels A Vector containing the texture coordinates
     */
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::function<void(shader*)> routine,

        std::vector<float> scaler,
        float rotator,
        std::vector<float> postitions,

        texture2D* texture,
        std::vector <float> texels);

    /**
     * @brief Destroy the game object
     */
    ~game_object();

    ///This function sets the shader routine to the given function
    void setShaderRoutine(std::function<void(shader*)> routine);

    ///This function sets the loop function to the given function
    void setLoopFunction(std::function<void(GLFWwindow* window, game_object* obj)> loop);

    ///This function returns a pointer to the attached shader
    shader* getShader();

    ///This function draws the object on the screen
    void draw();

    ///The loop function of the object
    void onLoop(GLFWwindow* window);

    ///false if no error has happened
    bool hasErrord();

    /**
     * @brief Get the Position of the object
     * 
     * @return std::vector<float> 
     */
    std::vector<float> getPosition();

    /**
     * @brief Set the Position of the object
     * 
     * @param pos a vector with the in x and y direction in world scale
     */
    void setPosition(std::vector<float> pos);

    /**
     * @brief moves the object by the specified amount
     * 
     * @param delta_pos a vector with the difference in x and y direction in world scale
     */
    void move(std::vector<float> delta_pos);

    ///gets the rotation in degrees
    float getRotation();

    ///sets the objects rotation by the specified amount in degrees counterclockwise
    ///@param rot the new rotation in degrees
    void setRotation(float rot);

    ///Rotates the object by the specified amount in degrees counterclockwise
    ///@param the difference in rotation in degrees
    void rotate(float delta_rot);

    ///set the alpha value of the color
    ///@param alpha The new alpha value of the color
    void setColorAlpha(float alpha);

    /**
     * Sets the screen size to let the object scale the texture correctly
     * 
     * @param width the width of the screen
     * @param height the height of he screen
     */
    void setScreenSize(int width, int height);

    /**
     * This function sets the name of the game_object to the given string.
     */
    void setName(std::string name);

    /**
     * This funtion returns the name of the object.
     */
    std::string getName();
};

/**
 * @brief Create a House object
 * 
 * @param texture A pointer to the texture that should be used (nullptr if none)
 * @param shade A pointer to the shader that should be used
 * @param texture_scale The factor by which the texture shoulb be scaled (1.0f if nothig is specified)
 * @return game_object* 
 */
game_object* createHouse(
    texture2D* texture,
    shader* shade,
    float texture_scale = 1.0f
);

/**
 * @brief Create a Circle object
 * 
 * @param midpoint The midpoint of the circle in world coordinates
 * @param radius The radius of the circle in world scale
 * @param edges The number of edges the circle should have (more make it look more round but take more performance)
 * @param innerColor The inner color of the circle (rgb)
 * @param outerColor The outer color of the circle (rgb)
 * @param shade A pointer to the shader that should be used
 * @param tex A pointer to the texture that should be used (nullptr if none)
 * @param texture_scale The factor by which the texture shoulb be scaled (1.0f if nothig is specified)
 * @return game_object* 
 */
game_object* createCircle( 
    float midpoint[2],
    float radius,
    int edges,
    float innerColor[3],
    float outerColor[3],
    shader* shade,
    texture2D* tex,
    float texture_scale = 1.0f);

/**
 * @brief Create a rectangular object
 * @param bottomleft The position of the bottom left Corner in World Coordinates
 * @param width The width of the rectangle in World Coordinates
 * @param height The width of the rectangle in World Coordinates
 * @param color The color of the rectangle (rgb)
 * @param shade A pointer to the shader that should be used
 * @param tex A pointer to the texture that should be used (nullptr if none)
 * @param DrawingMode The GL_DRAWING_MODE that should be used (eg. GL_DYNAMIC_DRAW)
 * @param textureScale The factor by which the texture shoulb be scaled (1.0f if nothig is specified)
 * @return game_object*
 */
game_object* createRecktangle(
    float bottomleft[2],
    float width,
    float height,
    float color[3],
    shader* shade,
    texture2D* tex,
    int DrawingMode,
    float textureScale = 1.0f);
