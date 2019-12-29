#pragma once

#include <vector>

#include "game_object.hpp"

class world{
private:
    std::vector <game_object*> WorldObjects;
    std::vector <shader*> WorldShaders;
    std::vector <texture2D*> WorldTextures;

    float cameraPosition[2] = {0.0f, 0.0f};

    int windowWidth = 600;
    int windowHeight = 200;

    glm::mat4 projectionMatrix;
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
    int addObject(game_object*);

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
    int removeObject(game_object* obj);
    
    /**
     * This function returns a pointer to the first shader with the given name
     * @param name the name of the wanted shader 
     * @return pointer to the shader if everything worked, nullptr if no shader was found or something went wrong
     */
    shader* getShaderByName(std::string name);

    /**
     * This function returns a pointer to the first texture with the given name
     * @param name the name of the wanted texture 
     * @return pointer to the texture if everything worked, nullptr if no shader was found or something went wrong
     */
    texture2D* getTextureByName(std::string name);

     /**
     * This function returns a pointer to the first object with the given name
     * @param name the name of the wanted object 
     * @return pointer to the object if everything worked, nullptr if no shader was found or something went wrong
     */
    game_object* getObjectByName(std::string name);

    /**
     * This function executes a world tick and is called for each frame.
     * All inputs are processed, the physics is calculated and all objects are rendered.
     * @param window the current active window where everything will be rendered
     */
    void tick(GLFWwindow* window);

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

    /**
     * This functions sets the window size to construct the projection matrix.
     * Every time the window size changess this function should be called in order to prevent unwanted stretching of all the objects.
     * @param width The width of the currently active window
     * @param height The height of the currently active window
     */
    void setWindowSize(int width, int height);
};