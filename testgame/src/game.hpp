#pragma once

#include "redhand/engine.hpp"
#include "redhand/complex_world.hpp"
#include "redhand/game_object.hpp"
#include "redhand/shader.hpp"
#include "redhand/texture.hpp"
#include "redhand/input.hpp"

int game_init(
    std::shared_ptr<redhand::complex_world> startWorld
);

int main_game_logic(
    redhand::game_loop_event evt
);

int createTestworld(std::shared_ptr<redhand::complex_world> testWorld);
int processHouseMovement(redhand::game_loop_event evt);
int processWorldInput(redhand::game_loop_event evt);
int processGlobalInput(redhand::game_loop_event evt);

std::unique_ptr<redhand::game_object> createHouse(
    std::shared_ptr<redhand::texture2D> texture,
    std::shared_ptr<redhand::shader> shade
);