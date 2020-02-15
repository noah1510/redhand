#pragma once

#include "redhand/engine.hpp"
#include "redhand/world.hpp"

int game_init(
    std::shared_ptr<world> startWorld
);

int main_game_logic(
    engine* gameEngine
);


int createTestworld(std::shared_ptr<world> testWorld);
void processHouseMovement(GLFWwindow* window, game_object* obj);
void processWorldInput(GLFWwindow* window, std::shared_ptr<world> activeWorld);
void processGlobalInput(engine* game);
