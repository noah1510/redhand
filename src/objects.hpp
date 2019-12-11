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

class object{
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
    std::function<void(GLFWwindow* window, object* obj)> LoopFunction;


    std::vector<float> object_position = {0.0f, 0.0f};
    std::vector<float> object_scale = {1.0f, 1.0f};
    float object_rotation = 0.0f;

    //the scale of the texture attached to this object
    glm::vec2 texture_scale = glm::vec2(1.0f, 1.0f);

public:
    //minimal constructor
    object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode);

    //minimal with shader routine
    object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::function<void(shader*)> routine);

    //minimal with scale,rotation and position
    object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::vector<float> scaler,
        float rotator,
        std::vector<float> postitions);

    //minimal with scale,rotation, position and shader routine
    object(
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
    object(
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
    object(
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

    ~object();

    //functions for the object

    void setShaderRoutine(std::function<void(shader*)> routine);
    void setLoopFunction(std::function<void(GLFWwindow* window, object* obj)> loop);

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
};


object* createHouse(
    texture2D* texture,
    shader* shade,
    float texture_scale = 1.0f
);

object* createCircle( 
    float midpoint[2],
    float radius,
    int edges,
    float innerColor[3],
    float outerColor[3],
    shader* shade,
    texture2D* tex,
    float texture_scale = 1.0f);

object* createRecktangle(
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