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

    /**
     * The constructor will create an empty world which can be filled with objects
     */
    world();

    /**
     * The destructor will clean up everything and delete all objects stored in this world.
     */
    ~world();

    /**
     * The addShader function will add a shader to the world.
     * Please add all shader you use for your objects to the world the objects resides in.
     * Not doing this may easily cause NULL Pointer exceptions and Segmentation faults.
     */
    int addShader(shader*);

    /**
     * The addTexture function will add a texture to the world.
     * Please add all testures you use for your objects to the world the objects resides in.
     * Not doing this may easily cause NULL Pointer exceptions and Segmentation faults.
     */
    int addTexture(texture2D*);

    /**
     * The addObject function will add an object to the world.
     * All added objects will be drawn on each world tick.
     */
    int addObject(object*);

    /**
     * This function removes a shader from its world and delete the shader.
     * @param shade a pointer to the shader which should be removed 
     * @return 0 if everything worked, 1 if no object was found, negative if something bad happened
    */
    int removeShader(shader* shade);

    /**
     * This function removes a texture from its world and delete the texture.
     * @param tex a pointer to the texture which should be removed 
     * @return 0 if everything worked, 1 if no object was found, negative if something bad happened
    */
    int removeTexture(texture2D* tex);

    /**
     * This function removes an object from its world and delete the object.
     * @param obj a pointer to the object which should be removed 
     * @return 0 if everything worked, 1 if no object was found, negative if something bad happened
    */
    int removeObject(object* obj);

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