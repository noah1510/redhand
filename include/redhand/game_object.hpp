#pragma once
#include "redhand/shader.hpp"
#include "redhand/texture.hpp"
#include "redhand/math.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <future>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace redhand{

/**
 * @brief This struct specifies all the properties of a game_object.
 * @note Please create a custom configuration by first setting your variable to redhand::DEFAULT_GAME_OBJECT_PROPERTIES.
 */
typedef struct{
    ///A vector containing all the points of the game_object, with each array being one point.
    std::vector <std::array<float, 2> >                     points_coordinates;
    ///A vector specifiying which points (their index) form an triangle, with each array being a triangle.
    std::vector <std::array<unsigned int, 3> >              triangle_indices;
    ///A vector specifiying the color of each point, with each array being one color.
    std::vector <std::array<float, 3> >                     point_colors;
    ///A vector containing all the texture coordinates of each point, with each array being one texture coordinate.
    std::vector <std::array<float, 2> >                     texture_coordinates;
    ///A shared pointer on the shader that should be used by this object.
    std::shared_ptr<redhand::shader>                        attached_shader;
    ///A shared pointer on the texture that should be used by this object.
    std::shared_ptr<texture2D>                              attached_texture;
    ///The scaling factor of the game_object along the x and y axis.
    std::array<float,2>                                     scale;
    ///The rotation around the bottom left in degrees.
    float                                                   rotation;
    ///The position of the object in world coordinates
    std::array<float,2>                                     postition;
    ///The drawing mode of the internal gpu buffer.
    int                                                     gl_drawing_mode;
    ///The name of the game_object
    std::string                                             name;
    ///The scaling factor of the attached texture along the x and y axis. 
    glm::vec2                                               texture_scale;
    ///THe alpha value of this game_object
    float                                                   alpha_value;
    
} game_object_properties;

const game_object_properties DEFAULT_GAME_OBJECT_PROPERTIES = {
    {{0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f}},
    {{0,1,2}},
    {{1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f}},
    {},
    nullptr,
    nullptr,
    {2.0f,2.0f},
    0,
    {-1.0f,-1.0f},
    GL_STATIC_DRAW,
    "game_object",
    {1.0f,1.0f},
    1.0f
};

/**
 * @brief The game_object is a simple object which can be displayed in a world.
 * It is the first abstraction layer and is a very low level openGL object.
 * You can create a game_object by specifying all the points, and which points form a triangel.
 * Each game_object NEEDS a shader in order to work.
 * Please use a shader which is added to the same world as the game_object in order to prevent errors.
 * 
 * The Points specified are all local coordinates.
 * This means they are in the first sector of a 2 dimensional coordinate system.
 * If there is any point outside of this range the creation of the object will fail.
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

    ///This array stores the data of all the vertecies
    std::vector <float> data;
    ///lock the point_data of the object
    std::shared_mutex mutex_data;

    ///The texture mode of the object
    unsigned int texture_mode = 0;
    ///lock the texture_mode of the object
    std::shared_mutex mutex_texture_mode;

    ///false if no error happened
    bool has_errored = false;
    ///lock the has_errored of the object
    std::shared_mutex mutex_has_errored;

    ///A function that will be called once every tick
    std::function<void(GLFWwindow* window, game_object* obj)> loop_function;
    ///lock the loop_function of the object
    std::shared_mutex mutex_loop_function;

    ///The properties of this object
    redhand::game_object_properties object_properties = redhand::DEFAULT_GAME_OBJECT_PROPERTIES;
    ///lock the properties of the object
    std::shared_mutex mutex_object_properties;

    ///The lock for the object
    std::shared_mutex mutex_game_object;

public:

    /**
     * @brief Construct a new game object object form a given set of properties.
     * @note the attached shader may not be a nullptr and at least one triangle should be specified.
     * @param properties The properties that should be used.
     */
    game_object(game_object_properties properties);

    /**
     * @brief Destroy the game object
     */
    ~game_object();

    ///This function sets the shader routine to the given function
    void setShaderRoutine(std::function<void(std::shared_ptr<redhand::shader>)> routine);

    /**
     * @brief Set the Loop Function which is called on every physics render call.
     * 
     * @param loop either a lambda function or a function pointer to the loop function that should be used.
     */
    void setLoopFunction(std::function<void(GLFWwindow* window, game_object* obj)> loop);

    ///This function returns a pointer to the attached shader
    std::shared_ptr<redhand::shader> getShader();

    ///This function draws the object on the screen
    void draw();

    ///The loop function of the object
    void onLoop(GLFWwindow* window);

    ///false if no error has happened
    bool hasErrord();

    void triggerError();

    /**
     * @brief Get the Position of the object
     * 
     * @return std::array<float,2> 
     */
    std::array<float,2> getPosition();

    /**
     * @brief Set the Position of the object
     * 
     * @param pos a vector with the in x and y direction in world scale
     */
    void setPosition(std::array<float,2> pos);

    /**
     * @brief moves the object by the specified amount
     * 
     * @param delta_pos a vector with the difference in x and y direction in world scale
     */
    void move(std::array<float,2> delta_pos);

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
    std::string_view getName();

    /**
     * @brief get the scal of the object
     * 
     * @return std::array<float,2> [0] = x scale and [1] is y scale
     */
    std::array<float,2> getScale();

};

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
std::unique_ptr<redhand::game_object> createCircle( 
    std::array<float,2> midpoint,
    float radius,
    unsigned int edges,
    std::array<float,3> innerColor,
    std::array<float,3> outerColor,
    std::shared_ptr<redhand::shader> shade,
    std::shared_ptr<redhand::texture2D> tex,
    float texture_scale = 1.0f
);

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
std::unique_ptr<redhand::game_object> createRectangle(
    std::array<float,2> bottomleft,
    float width,
    float height,
    std::array<float,3> color,
    std::shared_ptr<redhand::shader> shade,
    std::shared_ptr<redhand::texture2D> tex,
    int DrawingMode,
    float textureScale = 1.0f
);

}//end of namespace