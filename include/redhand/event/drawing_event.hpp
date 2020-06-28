#pragma once

#include "redhand/event/event.hpp"

namespace redhand{
    template <class T>
    class drawing_event : public redhand::event<T>{
        private:
        public:
            /**
             * @brief raise a new game loop event that is raised once every game loop tick.
             * This event is independet form Input events and drawing events and should be used to define game logic and physics.
             * 
             * @param raiser a pointer to the object that raised the event
             * @param delta_t the time difference to the last game_loop_event
             */
            drawing_event(T* raiser):event<T>(raiser){
            };

    };


}
