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

    //create the engine object
    engine* gameEngine = new engine();

    //get the current config of the engine
    engine_config conf = gameEngine->getConfig();
    
    //change the configuration and set the new config
    conf.title = "Redhand Test Game";
    gameEngine->setConfig(conf);

    //set the function which sets the initial state of the world
    exitCode = gameEngine->setFillWorldFunction(game_init);

    //set the function which handles all the controls and physics computation
    exitCode = gameEngine->setPhysicsLoopFunction(main_game_logic);

    //initilize the game engine
    gameEngine->init();

    //set the exit flags in case something went wrong 
    exitCode = gameEngine->getErrorCode();
    if(exitCode < 0){
        gameEngine->stopGame();
    }else{
        //run the game
        exitCode = gameEngine->runGame(); 
    }
    
    //let the destructor of the engine handle most of the cleanup
    delete gameEngine;
    
    //return the error code if something bad happened or 0 if everything is fine
    return abs(exitCode);
    
}




