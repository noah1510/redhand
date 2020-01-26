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

int engine::setPhysicsLoopFunction(int loop(engine*,world*,world*)){
    physicsLoopFunction = loop;

    return errorCode;
}

int engine::runGame(){
    //Clear up
    clearBuffers();

    //while there is no error run the game loop
    while(errorCode >= 0 ){

        //create a placeholder for the next world
        world* nextWorld = nullptr;

        //actuallly calculate the physics
        std::future<int> future_phys_error = std::async(physicsLoopFunction,this,activeWorld,nextWorld);
        
        //draw the game
        drawingFunction(this);

        //get the return code of the physics calculation
        errorCode = future_phys_error.get();
        
        //if there was an error terminate the game
        if(errorCode < 0){
            break;
        }

        //if not a nullptr change world
        if(nextWorld != nullptr){
            if(setActiveWorld(nextWorld) < 0){
                break;
            }
        }

        //close the game if the window should be closed
        if(glfwWindowShouldClose(window)){
            errorCode = 1;
            break;
        } 
        
    }

    return errorCode;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}