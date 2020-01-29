#include "engine.hpp"

engine::engine(){

    configuration = DEFAULT_ENGINE_CONFIG;
    
}

engine::~engine(){
    glfwSetWindowShouldClose(window, true);

    //try clearing up
    if(activeWorld != nullptr){
        try{
            delete activeWorld;
        }
        catch(const std::exception& e){
            std::cout << e.what() << '\n';
            errorCode = -7;
        }
    }

    //close the window + clean up
    glfwTerminate();
}

void engine::setConfig(engine_config conf){
    configuration = conf;
}

engine_config engine::getConfig(){
    return configuration;
}

void engine::init(){
    //init glfw
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, configuration.OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, configuration.OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, configuration.OPENGL_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, configuration.RESIZABLE);
    glfwWindowHint(GLFW_SAMPLES, configuration.SAMPLES);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, configuration.OPENGL_FORWARD_COMPAT); //needed for mac
  
    //create window
    window = glfwCreateWindow(configuration.window_width, configuration.window_height, configuration.title.c_str(), NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        errorCode = -1;
        return;
    }
    glfwMakeContextCurrent(window);

    //register callback function for viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    //make sure glad works
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        errorCode = -2;
        return;
    }

    //init stb
    initSTB();

    //enable multisampling
    glEnable(GL_MULTISAMPLE);

    //create an empty world
    activeWorld = new world();

    //fill that world
    if( ( errorCode = worldSetup(activeWorld) ) <= 0 ){
        return;
    }

    if(activeWorld == nullptr){
        errorCode = -4;
        return;
    }


}

world* engine::getActiveWorld(){
    if(errorCode < 0){
        return nullptr;
    }

    return activeWorld;
}

int engine::setActiveWorld(world* newWorld){
    
    if(newWorld != nullptr){
        if(activeWorld != nullptr){
            try{
                delete activeWorld;
            }
            catch(const std::exception& e){
                std::cout << e.what() << '\n';
                errorCode = -6;
            }
        }
        activeWorld = newWorld;
    }else
    {
        activeWorld = nullptr;
        errorCode = -7;
    }
    
    return errorCode;

}

void engine::clearBuffers(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLFWwindow* engine::getWindow(){
    return window;
}

int engine::getErrorCode(){
    return errorCode;
}

int engine::setFillWorldFunction( int fillFunction(world*) ){
    worldSetup = fillFunction;

    return errorCode;
}

int engine::setPhysicsLoopFunction(int loop(engine*)){
    physicsLoopFunction = loop;

    return errorCode;
}

bool engine::isRunning(){
    if (running){
        return true;
    }
    
    return false;
}

void engine::runPhysics(){
    while (isRunning()){

        //get the new error
        errorCode = physicsLoopFunction(this);

        //if there was an error terminate the game
        if(errorCode < 0){
            stopGame();
            break;
        }
    }
    
}

void engine::stopGame(){
    running = false;
}

std::function <int(engine*)> engine::getPhysicsFunction(){
    return physicsLoopFunction;
}

int engine::changeWorld(world* newWorld){
    //if not a nullptr change world
    if(newWorld == nullptr){
        stopGame();
        return -5;
        
    }
    errorCode = setActiveWorld(newWorld);
    if(errorCode < 0){
        stopGame();
        return -6;
    }
}

int engine::runGame(){
    running = true;

    //start the physics thread
    std::thread physicsThread([](engine* game){
        while (game->isRunning()){
            //create a placeholder for the next world
            world* nextWorld = nullptr;

            std::function <int(engine*)> func = game->getPhysicsFunction();
            //get the new error
            int errorCode = func(game);

            //if there was an error terminate the game
            if(errorCode < 0){
                game->stopGame();
                break;
            }

            //if not a nullptr change world
            if(nextWorld != nullptr){
                if(game->setActiveWorld(nextWorld) < 0){
                    game->stopGame();
                    break;
                }
            }
        }
    },this);

    //while there is no error run the game loop
    while(isRunning()){
        //Clear up
        clearBuffers();

        //draw the game
        drawingFunction(this);
          
    }

    physicsThread.join();

    return errorCode;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}