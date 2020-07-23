/**
 * @file world_switching_event.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref world_switching_event
 * @version 0.13
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

#include <redhand/event/event.hpp>

namespace redhand {

    class engine;
    class complex_world;

    /**
     * @brief This event is called after the game loop when the old world will be switched to a new world and after the new world was initilized.
     * This event can be used to move objects from an old world to a new one.
     * 
     */
    class world_switching_event : public redhand::event<engine> {
      private:
        /**
         * @brief This is a pointer to the world that will be deleted before the next game loop tick
         * 
         */
        std::shared_ptr<complex_world> previousWorld;

        /**
         * @brief This is a pointer to the world that will be activated before the next game loop tick
         * 
         */
        std::shared_ptr<complex_world> nextWorld;

      public:
        /**
         * @brief Construct a new world switching event
         * 
         * @param raiser a pointer to the object that raised the event
         * @param prev The world that was previously active
         * @param next The wolrd that will be activated next
         */
        world_switching_event(engine *raiser, std::shared_ptr<complex_world> prev, std::shared_ptr<complex_world> next) : event<engine>(raiser) {
            previousWorld = prev;
            nextWorld = next;
        };

        /**
         * @brief Get the Previous Wolrd 
         * 
         * @return std::shared_ptr<complex_world> A pointer to the previous world
         */
        std::shared_ptr<complex_world> getPreviousWolrd() {
            return previousWorld;
        }

        /**
         * @brief Get the Next Wolrd 
         * 
         * @return std::shared_ptr<complex_world> A pointer to the next world
         */
        std::shared_ptr<complex_world> getNextWolrd() {
            return nextWorld;
        }
    };

} // namespace redhand
