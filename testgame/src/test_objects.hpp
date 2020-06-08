#pragma once

#include "redhand/all.hpp"

class house : public redhand::game_object{
    public:
        house(
            std::shared_ptr<redhand::texture2D> texture,
            std::shared_ptr<redhand::shader> shade
        );

        void onLoop(redhand::game_loop_event evt);
};