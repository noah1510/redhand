#include "game.hpp"

int processGlobalInput(redhand::game_loop_event evt){

    if(evt.getRaiser() == nullptr){
        return -12;
    }

    if(redhand::input_system::static_isKeyPressed(redhand::KEY_ESCAPE)){
        evt.getRaiser()->stopGame();
    }

    /*if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }*/

    return 0;

}


