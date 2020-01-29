#pragma once

#include "engine.hpp"
#include "world.hpp"

int game_init(
    world* startWorld
);

int main_game_logic(
    engine* gameEngine
);


int createTestworld(world* testWorld);
void processHouseMovement(GLFWwindow* window, game_object* obj);
void processWorldInput(GLFWwindow* window, world* activeWorld);
void processGlobalInput(GLFWwindow* window);
