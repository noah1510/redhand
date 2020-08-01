/**
 * @file game_loop_event.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref game_loop_event
 * @version 0.13
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

#include "redhand/event/event.hpp"

#include <chrono>

namespace redhand {

    class engine;

    /**
     * @brief A game loop event is raised once every game loop tick.
     * This event is independed drawing events and should be used to define game logic and physics.
     * 
     */
    class game_loop_event : public redhand::event<engine> {
      private:
        ///The time difference compared to the last call of the game_loop.
        std::chrono::nanoseconds m_delta_t;

      public:
        /**
        * @brief raise a new game loop event that is raised once every game loop tick.
        * This event is independed drawing events and should be used to define game logic and physics.
        * 
        * @param raiser a pointer to the object that raised the event
        * @param delta_t the time difference to the last game_loop_event
        */
        game_loop_event(engine *raiser, std::chrono::nanoseconds delta_t) : event<engine>(raiser) {
            m_delta_t = delta_t;
        };

        /**
        * @brief Get the time difference between the last game loop tick and this one.
        * 
        * @return std::chrono::nanoseconds the time difference between the last event and the current one.
        */
        std::chrono::nanoseconds getFrameTime() {
            return m_delta_t;
        };
    };

} // namespace redhand
