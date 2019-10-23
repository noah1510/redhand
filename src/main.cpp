#include <glad/glad.h>
#include <gitGLFW/glfw3.h>

#include <gitglm/glm/glm.hpp>
#include <gitglm/glm/gtc/matrix_transform.hpp>
#include <gitglm/glm/gtc/type_ptr.hpp>

#include <iostream>
//#include <iterator>
//#include <chrono>
//#include <thread>
#include <cmath>
#include <string>
#include <vector>

#include "objects.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "game.hpp"
#include "world.hpp"
#include "audio/AudioHandler.hpp"

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void setWindowSize(int width, int height);

int main(){
    int exitCode = 0;

    //initilize the engine and exit if error
    //init glfw
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //needed for mac
  
    //create window
    GLFWwindow* window = glfwCreateWindow(600, 600, "Greenfoot++ v0.0.1", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //register callback function for viewport
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    //make sure glad works
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -2;
    }

    //init stb
    initSTB();

    //Create Vector containing all the worlds
    world* activeWorld = new world();
    exitCode = game_init(activeWorld);
    if(exitCode < 0){
        glfwSetWindowShouldClose(window, true);
        exitCode = -3;
    };
    if(activeWorld == nullptr){
        glfwSetWindowShouldClose(window, true);
        exitCode = -4;
    }
    
    //Clear up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //render loop
    while(!glfwWindowShouldClose(window)){
        //run the game loop
        world* nextWorld = nullptr;
        exitCode = game_loop(window, activeWorld, nextWorld);

        //if egative return exit loop
        if(exitCode < 0){
            break;
        }

        //if !nullptr change world
        if(nextWorld != nullptr){

            try{
                delete activeWorld;
            }
            catch(const std::exception& e){
                std::cout << e.what() << '\n';
                exitCode = -6;
                break;
            }
            activeWorld = nextWorld;
            
        }
        
    }

    glfwSetWindowShouldClose(window, true);

    //try clearing up
    try{
        delete activeWorld;
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }

    //close the window + clean up
    glfwTerminate();

    return exitCode;
}

void setWindowSize(int width, int height){
    glViewport(0, 0, width, height);
}

/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
*/
