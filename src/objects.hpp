#pragma once
#include "shader.hpp"
#include "texture.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <functional>

#include <glad/glad.h>
#include <gitGLFW/glfw3.h>

class object{
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    int indices_size;

    shader* shade;
    std::vector <texture2D*> textures;

    bool errored = false;
    
    std::function<void(shader*)> shader_routine;
    std::function<void(GLFWwindow* window, object* obj)> LoopFunction;

public:
    object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        std::vector <float> texels,
        shader* attached_shader,
        texture2D* texture,
        int gl_drawing_mode);
    object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        std::vector <float> texels,
        shader* attached_shader,
        texture2D* texture,
        int gl_drawing_mode,
        std::function<void(shader*)> routine);
    ~object();

    void setShaderRoutine(std::function<void(shader*)> routine);
    void setLoopFunction(std::function<void(GLFWwindow* window, object* obj)> loop);

    shader* getShader();

    void draw();
    void onLoop(GLFWwindow* window);
    bool hasErrord();
};


object* createHouse(
    texture2D* texture,
    shader* shade
);

object* createCircle( 
    float midpoint[3],
    float radius,
    int edges,
    float innerColor[3],
    float outerColor[3],
    shader* shade,
    texture2D* tex);

object* createColorTextureRectangle(
    float topRightCorner[3],
    float bottomLeftCorner[3],
    float colorTopRight[3],
    float colorBottomLeft[3],
    float texel2DTopRight[2],
    float texel2DBottomLeft[2],
    shader* shade,
    texture2D* tex,
    int DrawingMode);

float degToRad(float val);
float sinDeg(float deg);
float cosDeg(float deg);