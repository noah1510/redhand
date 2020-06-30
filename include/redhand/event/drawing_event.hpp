#pragma once

#include "redhand/event/event.hpp"

#include <vector>
#include <memory>
#include <typeinfo>

namespace redhand {
    class complex_world;
    class game_object;
    class shader;
    class drawing_event : public redhand::event<complex_world> {
      private:
        std::shared_ptr<shader> defaultShader;

      public:
        /**
        * @brief raise a new game loop event that is raised once every game loop tick.
        * This event is independet form Input events and drawing events and should be used to define game logic and physics.
        * 
        * @param raiser a pointer to the object that raised the event
        * @param delta_t the time difference to the last game_loop_event
        */
        drawing_event(complex_world *raiser, std::shared_ptr<shader> shade) : event<complex_world>(raiser) {
            defaultShader = shade;
        };

        std::shared_ptr<shader> getDefaultShader() {
            return defaultShader;
        };
    };

} // namespace redhand
