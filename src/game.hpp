#pragma once

#include "engine.hpp"
#include "world.hpp"

int game_init(
    std::shared_ptr<world> startWorld
);

int main_game_logic(
    std::shared_ptr<engine> gameEngine
);


int createTestworld(std::shared_ptr<world> testWorld);
void processHouseMovement(GLFWwindow* window, game_object* obj);
void processWorldInput(GLFWwindow* window, std::shared_ptr<world> activeWorld);
void processGlobalInput(std::shared_ptr<engine> game);
