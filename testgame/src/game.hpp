#pragma once

#include "redhand/engine.hpp"
#include "redhand/complex_world.hpp"

int game_init(
    std::shared_ptr<redhand::complex_world> startWorld
);

int main_game_logic(
    redhand::game_loop_event<redhand::engine> evt
);

int createTestworld(std::shared_ptr<redhand::complex_world> testWorld);
void processHouseMovement(GLFWwindow* window, redhand::game_object* obj);
void processWorldInput(GLFWwindow* window, std::shared_ptr<redhand::complex_world> activeWorld);
void processGlobalInput(redhand::engine* game);

std::unique_ptr<redhand::game_object> createHouse(
    std::shared_ptr<redhand::texture2D> texture,
    std::shared_ptr<redhand::shader> shade
);