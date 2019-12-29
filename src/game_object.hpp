#pragma once
#include "shader.hpp"
#include "texture.hpp"

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
 * If there is any point outside of this range the creation of teh object will fail (TODO).
 * For each point you must also specify the color of that point.
 * 
 * Each game_object has a name which is "game_object" by default.
 * 
 * In addition to that the object also holds its position, rotation and scale in world coordinates.
 */
class game_object{
private:

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    int indices_size;

    shader* object_shader;
    texture2D* object_texture;

    unsigned int textureMode = 0;
    float colorAlphaValue = 1.0f;

    bool errored = false;
    
    std::function<void(shader*)> shader_routine;
    std::function<void(GLFWwindow* window, game_object* obj)> LoopFunction;


    std::vector<float> object_position = {0.0f, 0.0f};
    std::vector<float> object_scale = {1.0f, 1.0f};
    float object_rotation = 0.0f;

    //the scale of the texture attached to this object
    glm::vec2 texture_scale = glm::vec2(1.0f, 1.0f);

    std::string object_name = "game_object";

public:
    //minimal constructor
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode);

    //minimal with shader routine
    game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::function<void(shader*)> routine);

    //minimal with scale,rotation and position
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

    ~game_object();

    //functions for the object

    void setShaderRoutine(std::function<void(shader*)> routine);
    void setLoopFunction(std::function<void(GLFWwindow* window, game_object* obj)> loop);

    shader* getShader();

    void draw();
    void onLoop(GLFWwindow* window);
    bool hasErrord();

    //sets and gets the object position
    std::vector<float> getPosition();
    void setPosition(std::vector<float> pos);
    void move(std::vector<float> delta_pos);

    //sets and gets the rotation in degrees
    float getRotation();
    void setRotation(float rot);
    void rotate(float delta_rot);

    //set the alpha value of the color
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


game_object* createHouse(
    texture2D* texture,
    shader* shade,
    float texture_scale = 1.0f
);

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
 * @param tex (optional) A pointer to the texture that should be used
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



float degToRad(float val);
float sinDeg(float deg);
float cosDeg(float deg);