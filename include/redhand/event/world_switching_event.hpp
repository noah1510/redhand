#pragma once

#include <redhand/event/event.hpp>

namespace redhand {

    class engine;
    class complex_world;

    class world_switching_event : public redhand::event<engine> {
      private:
        std::shared_ptr<complex_world> previousWorld;

        std::shared_ptr<complex_world> nextWorld;

      public:
        /**
        * @brief raise a new game loop event that is raised once every game loop tick.
        * This event is independet form Input events and drawing events and should be used to define game logic and physics.
        * 
        * @param raiser a pointer to the object that raised the event
        * @param prev The world that was previously active
        * @param next The wolrd that will be activated next
        * @param delta_t the time difference to the last game_loop_event
        */
        world_switching_event(engine *raiser, std::shared_ptr<complex_world> prev, std::shared_ptr<complex_world> next) : event<engine>(raiser) {
            previousWorld = prev;
            nextWorld = next;
        };

        std::shared_ptr<complex_world> getPreviousWolrd() {
            return previousWorld;
        }

        std::shared_ptr<complex_world> getNextWolrd() {
            return nextWorld;
        }
    };

} // namespace redhand
