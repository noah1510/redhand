#pragma once
#include "shader.hpp"
#include "texture.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <functional>

#include <gitglm/glm/glm.hpp>
#include <gitglm/glm/gtc/matrix_transform.hpp>
#include <gitglm/glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <gitGLFW/glfw3.h>

class object{
private:
    unsigned int objectVersion = 0;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    int indices_size;

    shader* shade;
    std::vector <texture2D*> textures;

    bool errored = false;
    
    std::function<void(shader*)> shader_routine;
    std::function<void(GLFWwindow* window, object* obj)> LoopFunction;


    std::vector<float> position = {0.0f, 0.0f};
    float scale = 1.0f;
    float rotation = 0.0f;

public:
    //minimal constructor
    object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        std::vector <float> texels,
        shader* attached_shader,
        texture2D* texture,
        int gl_drawing_mode);

    //constructor with shader_routine
    object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        std::vector <float> texels,
        shader* attached_shader,
        texture2D* texture,
        int gl_drawing_mode,

        std::function<void(shader*)> routine);

    //constructor for objectVersion 1
    object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        std::vector <float> texels,
        shader* attached_shader,
        texture2D* texture,
        int gl_drawing_mode,

        std::function<void(shader*)> routine,

        float scaler,
        float rotator,
        std::vector<float> postitions);

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