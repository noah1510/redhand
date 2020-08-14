/**
 * @file drawing_event.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref game_loop_event
 * @version 0.1.1
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

#include "redhand/event/event.hpp"

#include <vector>
#include <memory>
#include <typeinfo>

namespace redhand {
    class complex_world;
    class game_object;
    class shader;

    /**
     * @brief raise a new drawing event that is raised once every draw tick.
     * This event is independet form game loop events and should be used to draw the game_objects.
     * 
     */
    class drawing_event : public redhand::event<complex_world> {
      private:
        /**
         * @brief The default shader to render most objects
         * 
         */
        std::shared_ptr<shader> defaultShader;

      public:
        /**
        * @brief raise a new drawing event that is raised once every draw tick.
        * This event is independet form game loop events and should be used to draw the game_objects.
        * 
        * @param raiser a pointer to the object that raised the event
        * @param delta_t the time difference to the last game_loop_event
        */
        drawing_event(complex_world *raiser, std::shared_ptr<shader> shade) : event<complex_world>(raiser) {
            defaultShader = shade;
        };

        /**
         * @brief Get the Default Shader to render most objects
         * 
         * @return std::shared_ptr<shader> A shared pointer to the default shader
         */
        std::shared_ptr<shader> getDefaultShader() {
            return defaultShader;
        };
    };

} // namespace redhand
