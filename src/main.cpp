#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
//#include <iterator>
//#include <chrono>
//#include <thread>
#include <cmath>
#include <string>
#include <vector>

#include "game_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "game.hpp"
#include "world.hpp"
#include "engine.hpp"
//#include "audio/AudioHandler.hpp"



int main(){
    int exitCode = 0;

    //initilize the engine and exit if error
    engine* gameEngine = new engine();
    gameEngine->init();

    if(exitCode = gameEngine->getErrorCode()){
        glfwSetWindowShouldClose(gameEngine->getWindow(), true);
    }

    //Create the initial world
    gameEngine->setActiveWorld(new world());
    exitCode = game_init(gameEngine->getActiveWorld());
    if(exitCode < 0){
        glfwSetWindowShouldClose(gameEngine->getWindow(), true);
        exitCode = -3;
    };
    if(gameEngine->getActiveWorld() == nullptr){
        glfwSetWindowShouldClose(gameEngine->getWindow(), true);
        exitCode = -4;
    }
    
    //Clear up
    gameEngine->clearBuffers();

    //render loop
    while(exitCode < 0 || !glfwWindowShouldClose(gameEngine->getWindow())){
        //run the game loop
        world* nextWorld = nullptr;
        exitCode = game_loop(gameEngine->getWindow(), gameEngine->getActiveWorld(), nextWorld);

        //if negative return exit loop
        if(exitCode < 0){
            break;
        }

        //if !nullptr change world
        if(nextWorld != nullptr){
            if(gameEngine->setActiveWorld(nextWorld) < 0){
                break;
            }
        }
        
    }

    exitCode = gameEngine->getErrorCode();

    delete gameEngine;
    
    if(exitCode < 0){
        return -exitCode;
    }else{
        return exitCode;
    }  
    
}




