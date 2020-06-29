#pragma once

#include "redhand/all.hpp"

class house : public redhand::game_object{
    public:
        house(
            std::shared_ptr<redhand::texture2D> texture
        );

        void onLoop(redhand::game_loop_event evt);
};

class hand : public redhand::game_object{
    public:
        hand(
            std::shared_ptr<redhand::texture2D> texture
        );

        void onLoop(redhand::game_loop_event evt);

    protected:
        void updateWorldTransformation();
};