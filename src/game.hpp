#pragma once

#include "world.hpp"

int game_init(
    world* startWorld
);

int game_loop(
    GLFWwindow* window,
    world* activeWorld,
    world* nextWorld
);

int createTestworld(world* testWorld);
void processHouseMovement(GLFWwindow* window, object* obj);
void processWorldInput(GLFWwindow* window, world* activeWorld);
void processGlobalInput(GLFWwindow* window);
