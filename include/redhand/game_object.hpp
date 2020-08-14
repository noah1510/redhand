/**
 * @file game_object.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref game_object
 * @version 0.1.1
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once
#include "redhand/event/events.hpp"
#include "redhand/types.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <future>
#include <memory>
#include <thread>

namespace redhand {

    class texture2D;
    class shader;

    /**
    * @brief The game_object is a complex object which can be displayed in a world.
    * It is the first abstraction layer and is a very low level openGL object.
    * You can create a game_object by specifying all the points, and which points form a triangel.
    * If nothing else is specified, the default shader is used to render this object.
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
    class game_object {
      protected:
        /// The identifier of the Video Array Object
        unsigned int VAO = GL_INVALID_VALUE;

        ///The identifier of the Video Buffer Object
        unsigned int VBO = GL_INVALID_VALUE;

        ///The identifier of the Element Buffer Object
        unsigned int EBO = GL_INVALID_VALUE;

        ///This array stores the data of all the vertecies
        std::vector<float> data;
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

        ///The properties of this object
        redhand::game_object_properties object_properties;
        ///lock the properties of the object
        std::shared_mutex mutex_object_properties;

        ///The scaling factor of the attached texture along the x and y axis.
        glm::vec2 texture_scale = {1.0f, 1.0f};
        ///lock the texture_scale of the object
        std::shared_mutex mutex_texture_scale;

        ///The lock for the object
        std::shared_mutex mutex_game_object;

        ///The matrix for the world transformation (rotation,scaling and moving)
        glm::mat4 world_transformation = glm::mat4(1.0f);
        ///lock the world_transformation of the object
        std::shared_mutex mutex_world_transformation;

        /**
        * @brief this function updates the world_transformation matrix using the game_object_properties.
        * 
        */
        virtual void updateWorldTransformation();

        /**
        * @brief this function refreshed the gpu buffers
        * 
        */
        void updateBuffers();

        /**
        * @brief Construct an empty game_object, no buffers are initilized and not configuration set.
        * 
        */
        game_object();

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
        virtual ~game_object();

        /**
        * @brief Checks the provided properties for errors
        * 
        * @param properties the properties that should be checked
        * @return true the properties have an error
        * @return false there is no error in the properties
        */
        bool checkObjectProperties(game_object_properties properties);

        ///This function returns a pointer to the attached shader
        std::shared_ptr<redhand::shader> getShader();

        ///This function draws the object on the screen
        virtual void draw(redhand::drawing_event evt);

        ///The loop function of the object
        virtual void onLoop(game_loop_event evt);

        ///false if no error has happened
        bool hasErrord();

        virtual void triggerError();

        /**
        * @brief Get the Position of the object
        * 
        * @return std::array<float,2> 
        */
        virtual glm::vec2 getPosition();

        /**
        * @brief Set the Position of the object
        * 
        * @param pos a vector with the in x and y direction in world scale
        */
        virtual void setPosition(glm::vec2 pos);

        /**
        * @brief moves the object by the specified amount
        * 
        * @param delta_pos a vector with the difference in x and y direction in world scale
        */
        virtual void move(glm::vec2 delta_pos);

        ///gets the rotation in degrees
        virtual float getRotation();

        ///sets the objects rotation by the specified amount in degrees counterclockwise
        ///@param rot the new rotation in degrees
        virtual void setRotation(float rot);

        ///Rotates the object by the specified amount in degrees counterclockwise
        ///@param the difference in rotation in degrees
        virtual void rotate(float delta_rot);

        ///set the alpha value of the color
        ///@param alpha The new alpha value of the color
        virtual void setColorAlpha(float alpha);

        /**
        * Sets the screen size to let the object scale the texture correctly
        * 
        * @param width the width of the screen
        * @param height the height of he screen
        */
        virtual void setScreenSize(int width, int height);

        /**
        * This function sets the name of the game_object to the given string.
        */
        virtual void setName(std::string name);

        /**
        * This funtion returns the name of the object.
        */
        std::string_view getName();

        /**
        * @brief get the scal of the object
        * 
        * @return std::array<float,2> [0] = x scale and [1] is y scale
        */
        glm::vec2 getScale();

        /**
         * @brief Get the Hitbox of the game_object.
         * 
         * @return std::vector<glm::vec2> the hitbox of the game_object.
         */
        virtual std::vector<glm::vec2> getHitbox();

        //bool isAtEdge();
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
        glm::vec2 midpoint,
        float radius,
        unsigned int edges,
        glm::vec3 innerColor,
        glm::vec3 outerColor,
        std::shared_ptr<redhand::shader> shade,
        std::shared_ptr<redhand::texture2D> tex,
        std::string name,
        float texture_scale = 1.0f);

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
        glm::vec2 bottomleft,
        float width,
        float height,
        glm::vec3 color,
        std::shared_ptr<redhand::shader> shade,
        std::shared_ptr<redhand::texture2D> tex,
        redhand::drawing_modes DrawingMode,
        std::string name,
        float textureScale = 1.0f);

} // namespace redhand