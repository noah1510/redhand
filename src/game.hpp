#pragma once
#include <vector>

#include "world.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
void processGlobalInput(GLFWwindow* window);
