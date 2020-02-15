#include "redhand/engine.hpp"

engine::engine(){

    configuration = DEFAULT_ENGINE_CONFIG;
    
}

engine::~engine(){
    glfwSetWindowShouldClose(window, true);

    //try clearing up
    activeWorld.reset();

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
    activeWorld = std::shared_ptr<world> (new world());

    if(activeWorld == nullptr){
        errorCode = -4;
        return;
    }

}

std::shared_ptr<world> engine::getActiveWorld(){
    if(errorCode < 0){
        return nullptr;
    }

    return std::shared_ptr<world>(activeWorld);
}

int engine::setActiveWorld(std::shared_ptr<world> newWorld){
    
    if(newWorld != nullptr){
        activeWorld.reset();
        
        activeWorld = std::shared_ptr<world>(newWorld);
    }else{
        activeWorld = nullptr;
        stopGame(-7);
    }
    
    return -7;

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

int engine::setPhysicsLoopFunction(int loop(engine*)){
    physicsLoopFunction = loop;

    return errorCode;
}

bool engine::isRunning(){
    runningReadMutex.lock_shared();

    if (running){
        runningReadMutex.unlock_shared();
        return true;
    }
    runningReadMutex.unlock_shared();
    
    return false;

}


void engine::stopGame(int error){
    std::scoped_lock<std::shared_mutex> lock(runningReadMutex);

    if(error != 0){
        errorCode = error;
    }

    running = false;
}


int engine::changeWorld(std::shared_ptr<world> newWorld){
    //if not a nullptr change world
    if(newWorld == nullptr){
        stopGame(-5);
        return -5;
        
    }

    auto error = setActiveWorld(newWorld);
    if(error < 0){
        stopGame(error);
        return -6;
    }
}

int engine::runGame(){
    running = true;

    //start the physics thread
    std::thread physicsThread([&](){
        while (isRunning()){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            //create a placeholder for the next world
            std::shared_ptr<world> nextWorld = nullptr;

            //get the new error
            auto localErrorCode = physicsLoopFunction(this);

            //if there was an error terminate the game
            if(localErrorCode < 0){
                stopGame(localErrorCode);
                break;
            }

            //if not a nullptr change world
            if(nextWorld != nullptr){
                changeWorld(nextWorld);
            }

        }
    });

    //while there is no error run the game loop
    while(isRunning()){

        //draw the game
        
        //Clear up
        clearBuffers();

        //clear the bg color
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw the active world
        getActiveWorld()->draw();

        //Update the window buffers
        glfwSwapBuffers(getWindow());
        glfwPollEvents(); 

        std::this_thread::sleep_for(std::chrono::milliseconds(4));
          
    }

    physicsThread.join();

    return errorCode;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}