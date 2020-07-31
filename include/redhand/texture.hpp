/**
 * @file texture.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref texture2D
 * @version 0.13
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 * 
 */

#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vips/vips8>

#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <filesystem>

#include "redhand/math.hpp"
#include "redhand/types.hpp"

namespace redhand {

    void initImageLoader(char* str);


    /**
     * @brief This class is a simple 2 Dimensional texture.
     */
    class texture2D {
      private:
        ///true if an error happened
        bool errord = false;
        ///the mutex for the errord
        std::shared_mutex mutex_errored;

        ///the id of the texture
        unsigned int id = 0;
        ///the mutex for the id
        std::shared_mutex mutex_id;

        ///the width of the texture
        int width;
        ///the mutex for the width
        std::shared_mutex mutex_width;

        ///the hight of the texture
        int height;
        ///the mutex for the height
        std::shared_mutex mutex_height;

        ///the properties of the texture
        image_properties texture_properties;
        ///the mutex for the texture_properties
        std::shared_mutex mutex_texture_properties;

        ///initilizes the texture
        void initTexture2D();

        ///The underlying image data stored in the ram nullptr if none.
        ///This also allows for fast transformation of the texture without touching the shader.
        vips::VImage image_data;

        //std::unique_ptr<vips::VImage> image_data;

      public:
        /**
        * This constructor creates a 2-dimensional texture from a given filename.
        * @param prop The properties that should be used to create the image.
        */
        texture2D(image_properties prop);

        /**
        * @brief This constructor creates a 2-dimensional texture from a given filename and texture name.
        * All other properties are the DEFAULT_IMAGE_PROPERTIES
        * 
        * @param file_location the location of the file
        * @param texture_name the internale name of the texture
        */
        texture2D(std::string file_location, std::string texture_name);

        /**
        * @brief Destroy the texture2D object and cleans the memory
        * 
        */
        ~texture2D();

        ///This function returns true if an error has happened
        bool hasErrord();

        ///This function returns the ID of the texture so that it can be activated by shaders
        unsigned int getID();

        ///This function binds the texture to the specified texture unit
        ///@param unit The index of the texture unit the texture will be bound to (0 by default)
        void bind(int unit = 0);

        /**
        * This function retruns the width of the texture
         */
        int getWidth();

        /**
         * This function retruns the height of the texture
         */
        int getHeight();

        /**
         * This funtion returns the name of the object.
         */
        std::string_view getName();
    };

} // namespace redhand