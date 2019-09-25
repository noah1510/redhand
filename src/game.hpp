#pragma once
#include <vector>

#include "world.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int game_init(
    std::vector<world*>* worlds
);

int game_loop(
    GLFWwindow* window,
    world* activeWorld
);

int createTestworld(world* testWorld);
void processHouseMovement(GLFWwindow* window, object* obj);
void processGlobalInput(GLFWwindow* window);
