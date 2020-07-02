#pragma once

#include "redhand/engine.hpp"
#include "redhand/complex_world.hpp"
#include "redhand/shader.hpp"
#include "redhand/game_object.hpp"
#include "redhand/input.hpp"
#include "redhand/texture.hpp"
#include "game.hpp"
#include "test_objects.hpp"

class test_world : public redhand::complex_world{
    int onCreate(redhand::event<redhand::engine> evt);

    void tick(redhand::game_loop_event evt);
};
