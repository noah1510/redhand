#pragma once

#include <vector>

#include "objects.hpp"

class world{
private:
    std::vector <object*> WorldObjects;
    std::vector <shader*> WorldShaders;
    std::vector <texture2D*> WorldTextures;

    float cameraPosition[2] = {0.0f, 0.0f};
public:

    world();
    ~world();

    int addShader(shader*);
    int addTexture(texture2D*);
    int addObject(object*);

    int removeShader(int);
    int removeTexture(int);
    int removeObject(int);

    shader* getShaderByIndex(int);
    texture2D* getTextureByIndex(int);
    object* getObjectByIndex(int);

    shader* getShaderByName(std::string name);

    void tick(GLFWwindow* window);
    int test();

    /**
     * This function is used to set the position of the camera.
     * All units are in world space cooridinates.
     * The camera postition will be applied to all shaders inside of this world
     * @param pos_x The x coordinate of the camera
     * @param pos_y The y coordinate of the camera
     */
    void setCamera(float pos_x, float pos_y);

    /**
     * This function is used to set the position of the camera.
     * All units are in world space cooridinates
     * The camera postition will be applied to all shaders inside of this world
     * @param delta_pos_x The amount the camera should move along the x-Axis
     * @param delta_pos_y The amount the camera should move along the y-Axis
     */
    void moveCamera(float delta_pos_x, float delta_pos_y);
};