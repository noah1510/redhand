/**
 * @file actor.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref Actor
 * @version 0.13
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

#include "redhand/event/drawing_event.hpp"
#include "redhand/game_object.hpp"
#include "redhand/texture.hpp"
#include "redhand/types.hpp"
#include <chrono>
#include <filesystem>
#include <glm/glm.hpp>
#include <memory>
#include <shared_mutex>

namespace redhand {
    /**
     * @brief The Actor is the simple class used to diplay most things in redhand.
     * 
     */
    class Actor : private redhand::game_object {
        friend class redhand::complex_world;

      private:
        /**
          * @brief The mutex for locking the image.
          * 
          */
        std::shared_mutex mutex_image;

        /**
          * @brief This is the image attached to this actor and its size will determine the width of the actor
          * 
          */
        std::shared_ptr<redhand::texture2D> image;

        /**
          * @brief This is the onLoop function just calls act and is needed by the complexWorld for the gameLoopEvent.
          * 
          * @param evt The game_loop_event containing a pointer to the engine and a time difference
          */
        void onLoop(game_loop_event evt) override;

        /**
          * @brief The mutex for locking the actor scale.
          * 
          */
        std::shared_mutex mutex_actorScale;

        /**
          * @brief This specifies the height of the actor.
          * If there is no texture attached to the width is also the width. 
          * Otherwise it is determined by the scale of the image attached to this actor and then scaled by this factor.
          * 
          */
        float actorScale = 1.0f;

        /**
          * @brief The mutex for locking the movement Speed.
          * 
          */
        std::shared_mutex mutex_movementSpeed;

        /**
          * @brief This is the distance the actor should move per second.
          * 
          */
        float movementSpeed = 0.0f;

      protected:
        /**
          * @brief Set the Position of the actor to the given point
          * 
          * @param location the new position of the actor
          */
        void setPosition(glm::vec2 location) override;

        /**
         * @brief Set the Name of the Actor
         * 
         * @param name THe name the actor should have
         */
        void setName(std::string name) override;

        /**
          * @brief Set the alpha of this objects color
          * 
          * @param alpha value of this objects color
          */
        void setColorAlpha(float alpha) override;

        /**
          * @brief This allows you to change tha axis objects rotate around
          * 
          */
        void setRotaionAxis(glm::vec3);

        /**
          * @brief push new data into the opengl buffers
          * 
          * @param points the new set of points used to describe triangles
          * @param indicies the indixes that each form a single triangle
          * @param colors the colors each of the points should have (size must be 0 or size of points)
          */
        void updateBufferData(std::vector<glm::vec2> points, std::vector<std::array<unsigned int, 3>> indicies, std::vector<glm::vec3> colors);

      public:
        /**
          * @brief Construct a new Actor object with no image.
          * 
          */
        Actor();

        /**
          * @brief Construct a new Actor object with the supplied image.
          * 
          */
        Actor(redhand::texture2D *);

        /**
          * @brief The act method is called by the world to give actors a chance to perform some action.
          * 
          * @param evt This is the event, that holds a pointer to the engine and the time difference from the last call
          */
        virtual void act(game_loop_event evt);

        /**
          * @brief This returns the x position of the Actor in the world
          * 
          * @return float The x position
          */
        float getX();

        /**
          * @brief This returns the y position of the Actor in the world
          * 
          * @return float The y position
          */
        float getY();

        /**
          * @brief Set the Image of the Actor and automatically resizes accordingly
          * 
          * @param img The image that the Actor will now own
          */
        void setImage(std::shared_ptr<redhand::texture2D> img);

        /**
          * @brief Set the Image of the Actor and automatically resizes accordingly
          * 
          * @param img The location of the image the actor will use now
          */
        void setImage(std::filesystem::path img);

        /**
          * @brief Set the Image of the Actor and automatically resizes accordingly
          * 
          * @param img The location of the image the actor will use now
          */
        void setImage(redhand::image_properties img);

        /**
          * @brief scale the actor by the given factor
          * 
          * @param scale the scale the actor shuld have now
          */
        void setActorScale(float scale);

        /**
          * @brief scale the Actor by the given factor
          * 
          * @param scale the factor the actor will be scaled with
          */
        void scaleActor(float scale = 1.0f);

        /**
          * @brief Get the scaling factor of the actor
          * 
          * @return float the scale of the actor
          */
        float getActorScale();

        /**
          * @brief Get the size of the Actor (width and height)
          * 
          * @return glm::vec2 the dimensions of the Actor
          */
        glm::vec2 getSize();

        /**
          * @brief Get the Image attached to the actor
          * 
          * @return std::shared_ptr<redhand::texture2D> the image attached to the actor
          */
        std::shared_ptr<redhand::texture2D> getImage();

        /**
          * @brief Get the Rotation of the actor in degrees
          * 
          * @return float the rotation in degrees
          */
        float getRotation() override;

        /**
          * @brief Get the Position of the Actor
          * 
          * @return glm::vec2 The position in the world
          */
        glm::vec2 getPosition() override;

        /**
         * @brief Get the Name of the Actor
         * 
         * @return std::string the name of the actor
         */
        std::string_view getName();

        //bool isAtEdge() override;

        /**
          * @brief move the actor in the rotated direction by the specefied distance
          * 
          * @param distance The distance the actor should move
          */
        void move(float distance);

        /**
          * @brief move the actor using the specified speed.
          * This function is supposed to be called in the act function by supplying evt.getFrameTime() as parameter to this function.
          * 
          * @param frameTime The time difference between the function calls
          */
        void move(std::chrono::nanoseconds frameTime);

        /**
          * @brief move the actor by a given amount of units
          * 
          * @param delta the units the actor should move
          */
        void move(glm::vec2 delta) override;

        /**
          * @brief Set the movement Speed of the Actor in units per seconds
          * 
          * @param speed the speed in units per second
          */
        void setSpeed(float speed);

        /**
          * @brief Turn the actor by a given amount of degrees
          * 
          * @param delta how much degrees the actor should rotate
          */
        void turn(float delta);

        /**
          * @brief Specify a point the actor should turn towards
          * 
          * @param point the point the actor should look at
          */
        void turnTowards(glm::vec2 point);
    };
} // namespace redhand
