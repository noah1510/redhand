#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
//#include <iterator>
//#include <chrono>
//#include <thread>
#include <cmath>
#include <string>
#include <vector>

#include "game_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "game.hpp"
#include "world.hpp"
//#include "audio/AudioHandler.hpp"

///This function will be called every time the window size is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

typedef struct{
    int OPENGL_VERSION_MAJOR;
    int OPENGL_VERSION_MINOR;
    int OPENGL_PROFILE;
    int SAMPLES;
    int RESIZABLE;
    int OPENGL_FORWARD_COMPAT;
    int window_width;
    int window_height;
    std::string title;
}engine_config;

const engine_config DEFAULT_ENGINE_CONFIG = {
    3,
    3,
    GLFW_OPENGL_CORE_PROFILE,
    4,
    GL_FALSE,
    GL_TRUE,
    600,
    600,
    "RedHand v0.0.1"
};
/**
 * @brief This class basically handles all the setup need for games to work.
 * 
 */
class engine{
private:
    ///The opengl configuration of the engine
    engine_config configuration;

    ///a pointer to the active window
    GLFWwindow* window;

    ///an error code which hopefully stays 0 but will be negataiv if something bad happened
    int errorCode = 0;

    ///The currently active world (nullptr or empty world if none)
    world* activeWorld;

    std::function <int(world*)> worldSetup;

    std::function <int(GLFWwindow*,world*,world*)> physicsLoopFunction;
public:
    ///The constructor for the engine which constructs the object
    engine();

    ///The destructor for the engine which cleans everything up
    ~engine();

    /**
     * @brief Set the Config of the game engine to the given configuration
     * 
     * @param conf The configuration which sould be used (by default DEFAULT_ENGINE_CONFIG)
     */
    void setConfig(engine_config conf);

    /**
     * @brief Get the current configuration of the engine
     * 
     * @return engine_config The current configuration of the engine
     */
    engine_config getConfig();

    /**
     * @brief This function initilizes the engine like specified in the configuration.
     * 
     */
    void init();

    /**
     * @brief Set the Active World object to the given world
     * 
     * @param newWorld a pointer to the world that will be the new active world
     * @return int negative if someting went wrong
     */
    int setActiveWorld(world* newWorld);

    /**
     * @brief Get the Active World object
     * 
     * @return world* A pointer to the currently active world
     */
    world* getActiveWorld();

    /**
     * @brief Get the Window object
     * 
     * @return * window a pointer to the currently active window
     */
    GLFWwindow* getWindow();

    /**
     * @brief this function returns the error code
     * 
     * @return negative if something bad happened
     */
    int getErrorCode();

    /**
     * @brief This functions clears the currently bound buffers.
     * 
     */
    void clearBuffers();

    /**
     * @brief This function specifies which function to use to fill the initial world of the game
     * 
     * @param fillFunction a function which should be used to fill the initial world of the game. It should a negative number is something went wrong and it needs a pointer to an empty world which will be filled as an argument.
     * @return int a negative value if something went wrong
     */
    int setFillWorldFunction( int fillFunction(world*) );

    /**
     * @brief Set the Loop Function of the engine.
     * The loop function is the function responible for handeling all the inputs and calcualting all the physics
     * 
     * @param loop The loop function which returns a negative number if something went wrong and has three parameters, the currently active window, the currently active world and a pointer to the world which should be used next, which is a nullptr if there should be no change,
     * @return int the errorCode of the engine will be returned, negative if something bad happened
     */
    int setPhysicsLoopFunction( int loop(GLFWwindow*,world*,world*) );

    /**
     * @brief This function runs the game, the engine handles all the logic to keep everything wunning for you.
     * @warning This function runs until the game is finished and the game should terminate.
     * 
     * @return int negative if something bad happend, otherwise a positive return code
     */
    int runGame();
};


