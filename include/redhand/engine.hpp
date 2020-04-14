#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
//#include <iterator>
#include <chrono>
#include <cmath>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <memory>
#include <algorithm>
#include <mutex>
#include <shared_mutex>

#include "redhand/game_object.hpp"
#include "redhand/shader.hpp"
#include "redhand/texture.hpp"
#include "redhand/world.hpp"
#include "redhand/math.hpp"
#include "redhand/AudioHandler.hpp"

namespace redhand{

const auto OPENGL_CORE_PROFILE = GLFW_OPENGL_CORE_PROFILE;
const auto DONT_CARE = GLFW_DONT_CARE;

///This string provides a version in a pritable format.
///The first public version is 0.1.0 and from there it will be couted up.
///There might be subversions in the format "X.Y.Z" but the Z only tells how much further the current build is from the last major release
const auto REDHAND_VERSION_STRING = "0.0.6";

///This function will be called every time the window size is changed
void framebuffer_size_callback(GLFWwindow*, int width, int height);

/**
 * @brief This struct specifies all the properties of the game engine.
 * @note Please create a custom configuration by first setting your variable to redhand::DEFAULT_ENGINE_CONFIG.
 */
typedef struct{
    ///The newest version of OpenGL which may be used
    int OPENGL_VERSION_MAJOR;
    ///The oldest version of OpenGL which may be used
    int OPENGL_VERSION_MINOR;
    ///The profile OpenGL should run in (should be redhand::OPENGL_CORE_PROFILE (0x00032001) )
    int OPENGL_PROFILE;
    ///Specifies the desired number of samples to use for multisampling. 
    ///Zero disables multisampling. A value of (redhand::DONT_CARE (-1) ) means the application has no preference.
    int SAMPLES;
    ///true if window should be resizable false if not (will be ignored in fullscreen or not decorated)
    bool RESIZABLE;
    ///specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed. 
    int OPENGL_FORWARD_COMPAT;
    ///the width of the window
    unsigned int window_width;
    ///the heigth of the window
    unsigned int window_height;
    ///the window title
    std::string title;
}engine_config;


///The default configuration of the engine.
const engine_config DEFAULT_ENGINE_CONFIG = {
    3,
    3,
    redhand::OPENGL_CORE_PROFILE,
    4,
    GL_FALSE,
    GL_TRUE,
    600,
    600,
    redhand::REDHAND_VERSION_STRING
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
    std::shared_ptr<world> activeWorld;

    ///The function which is executed on each physics tick
    std::function <int(engine*)> physicsLoopFunction;

    std::shared_mutex runningReadMutex;

    bool running = false;
    //std::thread physicsThread;

    //std::thread drawingThread;
public:
    ///The constructor for the engine which constructs the object
    engine();

    ///The destructor for the engine which cleans everything up
    ~engine();

    /**
     * @brief Set the Config of the game engine to the given configuration
     * @version Available since REDHAND_VERSION_NUMBER 1
     */
    void setConfig(engine_config conf);

    /**
     * @brief Get the current configuration of the engine
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @return engine_config The current configuration of the engine
     */
    engine_config getConfig();

    /**
     * @brief This function initilizes the engine like specified in the configuration.
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     */
    void init();

    /**
     * @brief Set the Active World object to the given world
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @param newWorld a pointer to the world that will be the new active world
     * @return int negative if someting went wrong
     */
    int setActiveWorld(std::shared_ptr<world> newWorld);

    /**
     * @brief change the world to the given new world
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @param newWorld a shared pointer to the new world. An error will accour if it is a nullptr.
     * @return int 
     */
    int changeWorld(std::shared_ptr<world> newWorld);

    /**
     * @brief Get the Active World object
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @return std::shared_ptr<world> A pointer to the currently active world
     */
    std::shared_ptr<world> getActiveWorld();

    /**
     * @brief Get the Window object
     * @warning will be deprecated in the future only giving access to feautures over methods of the engine
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @return GLFWwindow* a pointer to the currently active window
     */
    GLFWwindow* getWindow();

    /**
     * @brief this function returns the error code
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @return negative if something bad happened
     */
    int getErrorCode();

    /**
     * @brief This functions clears the currently bound buffers.
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     */
    void clearBuffers();

    /**
     * @brief Set the Loop Function of the engine.
     * @detail The loop function is the function responible for handeling all the inputs and calcualting all the physics
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @param loop The loop function which returns a negative number if something went wrong and has one parameter, which is a raw pointer to the engine object.
     * @warning Do no delete the engine object or any of its members inside the physics loop, always modify them using the methods.
     * @return int the errorCode of the engine will be returned, negative if something bad happened
     */
    int setPhysicsLoopFunction( int loop(engine*) );

    /**
     * @brief This function runs the game, the engine handles all the logic to keep everything wunning for you.
     * @warning This function runs until the game is finished and the game should terminate.
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @return int negative if something bad happend, otherwise a positive return code
     */
    int runGame();

    /**
     * @brief returns true if the game is running
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @return true running
     * @return false not running
     */
    bool isRunning();

    /**
     * @brief stops the game when called
     * @version Available since REDHAND_VERSION_NUMBER 1
     * 
     * @param error the error code which the game should be set to, 0 if none is given.
     */
    void stopGame(int error = 0);

};

}//end of namespace
