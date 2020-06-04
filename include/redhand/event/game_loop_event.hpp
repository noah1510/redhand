#pragma once

#include "redhand/event/event.hpp"

#include <chrono>

namespace redhand{
    template <class T>
    class game_loop_event : redhand::event<T>{
        private:
            ///The time difference compared to the last call of the game_loop.
            std::chrono::nanoseconds m_delta_t;
        public:
            /**
             * @brief raise a new game loop event that is raised once every game loop tick.
             * This event is independet form Input events and drawing events and should be used to define game logic and physics.
             * 
             * @param raiser a pointer to the object that raised the event
             * @param delta_t the time difference to the last game_loop_event
             */
            game_loop_event(T* raiser, std::chrono::nanoseconds delta_t):event<T>(raiser){
                m_delta_t = delta_t;
            };

            /**
             * @brief Get the time difference between the last game loop tick and this one.
             * 
             * @return std::chrono::nanoseconds the time difference between the last event and the current one.
             */
            std::chrono::nanoseconds getFrameTime(){
                return m_delta_t;
            };

    };


}
