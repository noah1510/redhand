/**
 * @file types.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief types that are used throughout the redhand library
 * @version 0.1.1
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

/* If we are we on Windows, use dll
 */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
    #define GLFW_DLL
#endif

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <filesystem>
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <future>
#include <memory>
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

namespace redhand {

    class shader;
    class texture2D;

    const auto OPENGL_CORE_PROFILE = GLFW_OPENGL_CORE_PROFILE;
    const auto DONT_CARE = GLFW_DONT_CARE;

    /**
     * @brief This type specifies the actions a key can trigger
     * 
     */
    enum key_actions {
        PRESS = GLFW_PRESS,
        REPEAT = GLFW_REPEAT,
        RELEASE = GLFW_RELEASE,
        UNKNOWN = GLFW_KEY_UNKNOWN
    };

    /**
     * @brief This type contains all keys on the keyboard
     * 
     */
    enum keyboard_keys {
        KEY_UNKNOWN = GLFW_KEY_UNKNOWN,
        KEY_SPACE = GLFW_KEY_SPACE,
        KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE,
        KEY_COMMA = GLFW_KEY_COMMA,
        KEY_MINUS = GLFW_KEY_MINUS,
        KEY_PERIOD = GLFW_KEY_PERIOD,
        KEY_SLASH = GLFW_KEY_SLASH,
        KEY_0 = GLFW_KEY_0,
        KEY_1 = GLFW_KEY_1,
        KEY_2 = GLFW_KEY_2,
        KEY_3 = GLFW_KEY_3,
        KEY_4 = GLFW_KEY_4,
        KEY_5 = GLFW_KEY_5,
        KEY_6 = GLFW_KEY_6,
        KEY_7 = GLFW_KEY_7,
        KEY_8 = GLFW_KEY_8,
        KEY_9 = GLFW_KEY_9,
        KEY_SEMICOLON = GLFW_KEY_SEMICOLON,
        KEY_EQUAL = GLFW_KEY_EQUAL,
        KEY_A = GLFW_KEY_A,
        KEY_B = GLFW_KEY_B,
        KEY_C = GLFW_KEY_C,
        KEY_D = GLFW_KEY_D,
        KEY_E = GLFW_KEY_E,
        KEY_F = GLFW_KEY_F,
        KEY_G = GLFW_KEY_G,
        KEY_H = GLFW_KEY_H,
        KEY_I = GLFW_KEY_I,
        KEY_J = GLFW_KEY_J,
        KEY_K = GLFW_KEY_K,
        KEY_L = GLFW_KEY_L,
        KEY_M = GLFW_KEY_M,
        KEY_N = GLFW_KEY_N,
        KEY_O = GLFW_KEY_O,
        KEY_P = GLFW_KEY_P,
        KEY_Q = GLFW_KEY_Q,
        KEY_R = GLFW_KEY_R,
        KEY_S = GLFW_KEY_S,
        KEY_T = GLFW_KEY_T,
        KEY_U = GLFW_KEY_U,
        KEY_V = GLFW_KEY_V,
        KEY_W = GLFW_KEY_W,
        KEY_X = GLFW_KEY_X,
        KEY_Y = GLFW_KEY_Y,
        KEY_Z = GLFW_KEY_Z,
        KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
        KEY_BACKSLASH = GLFW_KEY_BACKSLASH,
        KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
        KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
        KEY_WORLD_1 = GLFW_KEY_WORLD_1,
        KEY_WORLD_2 = GLFW_KEY_WORLD_2,
        KEY_ESCAPE = GLFW_KEY_ESCAPE,
        KEY_ENTER = GLFW_KEY_ENTER,
        KEY_TAB = GLFW_KEY_TAB,
        KEY_BACKSPACE = GLFW_KEY_BACKSPACE,
        KEY_INSERT = GLFW_KEY_INSERT,
        KEY_DELETE = GLFW_KEY_DELETE,
        KEY_RIGHT = GLFW_KEY_RIGHT,
        KEY_LEFT = GLFW_KEY_LEFT,
        KEY_DOWN = GLFW_KEY_DOWN,
        KEY_UP = GLFW_KEY_UP,
        KEY_PAGE_UP = GLFW_KEY_PAGE_UP,
        KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
        KEY_HOME = GLFW_KEY_HOME,
        KEY_END = GLFW_KEY_END,
        KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
        KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
        KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK,
        KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
        KEY_PAUSE = GLFW_KEY_PAUSE,
        KEY_F1 = GLFW_KEY_F1,
        KEY_F2 = GLFW_KEY_F2,
        KEY_F3 = GLFW_KEY_F3,
        KEY_F4 = GLFW_KEY_F4,
        KEY_F5 = GLFW_KEY_F5,
        KEY_F6 = GLFW_KEY_F6,
        KEY_F7 = GLFW_KEY_F7,
        KEY_F8 = GLFW_KEY_F8,
        KEY_F9 = GLFW_KEY_F9,
        KEY_F10 = GLFW_KEY_F10,
        KEY_F11 = GLFW_KEY_F11,
        KEY_F12 = GLFW_KEY_F12,
        KEY_F13 = GLFW_KEY_F13,
        KEY_F14 = GLFW_KEY_F14,
        KEY_F15 = GLFW_KEY_F15,
        KEY_F16 = GLFW_KEY_F16,
        KEY_F17 = GLFW_KEY_F17,
        KEY_F18 = GLFW_KEY_F18,
        KEY_F19 = GLFW_KEY_F19,
        KEY_F20 = GLFW_KEY_F20,
        KEY_F21 = GLFW_KEY_F21,
        KEY_F22 = GLFW_KEY_F22,
        KEY_F23 = GLFW_KEY_F23,
        KEY_F24 = GLFW_KEY_F24,
        KEY_F25 = GLFW_KEY_F25,
        KEY_KP_0 = GLFW_KEY_KP_0,
        KEY_KP_1 = GLFW_KEY_KP_1,
        KEY_KP_2 = GLFW_KEY_KP_2,
        KEY_KP_3 = GLFW_KEY_KP_3,
        KEY_KP_4 = GLFW_KEY_KP_4,
        KEY_KP_5 = GLFW_KEY_KP_5,
        KEY_KP_6 = GLFW_KEY_KP_6,
        KEY_KP_7 = GLFW_KEY_KP_7,
        KEY_KP_8 = GLFW_KEY_KP_8,
        KEY_KP_9 = GLFW_KEY_KP_9,
        KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
        KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
        KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
        KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
        KEY_KP_ADD = GLFW_KEY_KP_ADD,
        KEY_KP_ENTER = GLFW_KEY_KP_ENTER,
        KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL,
        KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
        KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
        KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT,
        KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
        KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
        KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
        KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
        KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
        KEY_MENU = GLFW_KEY_MENU,
        KEY_LAST = GLFW_KEY_LAST
    };

    /**
     * @brief This struct stores all the information needed to create a 2 dimensional image from a file.
     * 
     */
    class image_properties {
      public:
        ///The name of the image
        std::string name = "texture";

        ///The location of the image file
        std::filesystem::path file_location;

        ///The wrap mode in x direction
        int wrap_S = GL_TEXTURE_WRAP_S;

        ///The wrap mode in y direction
        int wrap_T = GL_TEXTURE_WRAP_T;

        ///The scaling type when downscaling the image
        int texture_min_filter = GL_LINEAR_MIPMAP_LINEAR;

        ///The scaling type when upscaling the image
        int texture_max_filter = GL_LINEAR;

        ///The data type of the image
        int data_type = GL_RGBA;

        ///True if the program should create mipmaps
        bool create_mipmap = true;

        ///The internal data type that should be used
        int internal_data_type = GL_RGBA;

        ///If flase image_data will always be a nullptr.
        ///This allows for much less memory usage but before transforming the texture the image has to be copied from the gpu to the ram costing some time.
        bool keep_image_data = true;

    };

    /**
     * @brief These are the available opengl drawing modes 
     * 
     */
    enum drawing_modes {
        STATIC_DRAW = 0x88E4,
        DYNAMIC_DRAW = 0x88E8,
    };

    /**
     * @brief This struct specifies all the properties of a game_object.
     */
    class game_object_properties {
      public:
        ///A vector containing all the points of the game_object, with each array being one point.
        std::vector<glm::vec2> points_coordinates = {{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}};
        ///A vector specifiying which points (their index) form an triangle, with each array being a triangle.
        std::vector<std::array<unsigned int, 3>> triangle_indices = {{0, 1, 2}};
        ///A vector specifiying the color of each point, with each array being one color.
        std::vector<glm::vec3> point_colors = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
        ///A vector containing all the texture coordinates of each point, with each array being one texture coordinate.
        std::vector<glm::vec2> texture_coordinates = {};
        ///A shared pointer on the shader that should be used by this object.
        std::shared_ptr<redhand::shader> attached_shader = nullptr;
        ///A shared pointer on the texture that should be used by this object.
        std::shared_ptr<texture2D> attached_texture = nullptr;
        ///The scaling factor of the game_object along the x and y axis.
        glm::vec2 scale = {2.0f, 2.0f};
        ///The rotation around the bottom left in degrees.
        float rotation = 0.0f;
        ///The position of the object in world coordinates
        glm::vec2 postition = {-1.0f, -1.0f};
        ///The drawing mode of the internal gpu buffer.
        drawing_modes gl_drawing_mode = STATIC_DRAW;
        ///The name of the game_object
        std::string name = "game_object";
        ///The scaling factor of the attached texture along the x and y axis.
        ///@note this is not the real texture scale this is more like a multiplier for it.
        glm::vec2 texture_scale = {1.0f, 1.0f};
        ///THe alpha value of this game_object
        float alpha_value = 1.0f;
        ///Enable automatic scaling of texture (might be buggy)
        bool automatic_scaling = false;
        ///The postion of the rotation axis on the object
        glm::vec2 rotation_point = {0.5f, 0.5f};
        ///The position of the rotation axis in the 3D Space (only the angle and orientation matters)
        glm::vec3 rotation_axis = {0.0f,0.0f,1.0f};

        /**
        * @brief Checks the properties for errors
        * 
        * @param properties the properties that should be checked
        * @return true the properties have an error
        * @return false there is no error in the properties
        */
        bool check();

        /**
         * @brief create the vector containing all the vertex data
         * 
         * @return std::vector<float> the vertex data
         */
        std::vector<float> getData();

        /**
         * @brief Get the Indicies for the EBO in one single vector
         * 
         * @return std::vector<unsigned int> the indicies in a single vector
         */
        std::vector<unsigned int> getIndicies();

    } ;

    /**
     * @brief This struct specifies all the properties of the game engine.
     */
    class engine_config{
      public:
        //The newest version of OpenGL which may be used
        int OPENGL_VERSION_MAJOR = 3;
        ///The oldest version of OpenGL which may be used
        int OPENGL_VERSION_MINOR = 3;
        ///The profile OpenGL should run in (should be redhand::OPENGL_CORE_PROFILE (0x00032001) )
        int OPENGL_PROFILE = redhand::OPENGL_CORE_PROFILE;
        ///Specifies the desired number of samples to use for multisampling.
        ///Zero disables multisampling. A value of (redhand::DONT_CARE (-1) ) means the application has no preference.
        int SAMPLES = 4;
        ///true if window should be resizable false if not (will be ignored in fullscreen or not decorated)
        bool RESIZABLE = false;
        ///specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed.
        int OPENGL_FORWARD_COMPAT = 1;
        ///the width of the window
        unsigned int window_width = 600;
        ///the heigth of the window
        unsigned int window_height = 600;
        ///the window title
        std::string title = "redhand application";
        ///the current redhand major version X.y.z
        unsigned int redhand_version_major = 0;
        ///the current redhand minor version x.Y.z
        unsigned int redhand_version_minor = 1;
        ///the current redhand pacht version x.y.Z
        unsigned int redhand_version_patch = 2;

        /**
         * @brief check if the the version of the engine matches the given version
         * 
         * @param major the major version number X.y.z
         * @param minor the minor version number x.Y.z
         * @param patch the patch version number x.y.Z
         * @return true the version match
         * @return false the version is not exactly the same
         */
        bool doesMatchVersion(unsigned int major, unsigned int minor, unsigned int patch);

        /**
         * @brief check if the version of the engine is greater or equal the given value
         * 
         * @param major the major version number X.y.z
         * @param minor the minor version number x.Y.z
         * @param patch the patch version number x.y.Z
         * @return true the version of the engine is greater or equal to the given version
         * @return false the version of the engine is lower than the given value
         */
        bool versionIsGreaterThan(unsigned int major, unsigned int minor, unsigned int patch);

        /**
         * @brief check if the version of the engine is greater or equal the given value
         * 
         * @param version the version in the format x.y.z
         * @return true the version of the engine is greater or equal to the given version
         * @return false the version of the engine is lower than the given value
         */
        bool versionIsGreaterThan(std::string version);
    };

} // namespace redhand