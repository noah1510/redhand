#include "game.hpp"

int game_init(
    std::shared_ptr<redhand::world> startWorld
){
    int exitCode = 0;

    exitCode = createTestworld(startWorld);
    if(exitCode < 0){
        return exitCode;
    }

    return 0;   
}

int main_game_logic(
    redhand::engine* gameEngine
){
    //process the input
    processGlobalInput(gameEngine);
    processWorldInput(gameEngine->getWindow(), gameEngine->getActiveWorld());

    //get the current window size
    int width, height;
    glfwGetWindowSize(gameEngine->getWindow(), &width, &height);
    gameEngine->getActiveWorld()->setWindowSize(width, height);

    //tick the active world
    gameEngine->getActiveWorld()->tick(gameEngine->getWindow());

    return 0;

}

int createTestworld(std::shared_ptr<redhand::world> testWorld){
    //add shaders to world

    auto shader1 =  std::shared_ptr<redhand::shader>( new redhand::shader() );
    if( testWorld->addShader(shader1) < 0){
        std::cerr << "Got error while adding shader" << std::endl;
        return -1;
    }

    if(testWorld->getShaderByName("default") == nullptr){
        std::cerr << "Got nullpointer as shader" << std::endl;
        return -20;
    }

    //Add textures to world
    if(testWorld->addTexture(
        new redhand::texture2D(
            "textures/open/crate.png",
            "house"
        )) < 0){
        return -2;
    }

    if(testWorld->addTexture(
        new redhand::texture2D(
            "textures/open/house.png",
            "bg"
        )) < 0){
        return -2;
    }
    
    //creating the objects and add them to the world
    int edges = 240;

     // Background rectangle
    {
        float bottomLeft[2] = {-100.0f, -100.0f};
        float color[3] = {0.0f, 0.6f, 1.0f};
        if( testWorld->addObject(
            redhand::createRecktangle(
                bottomLeft,
                200.0f,
                200.0f,
                color,
                testWorld->getShaderByName("default"),
                testWorld->getTextureByName("bg"),
                GL_STATIC_DRAW,
                600.0f*200.0f/testWorld->getTextureByName("bg")->getWidth()
            )
        ) < 0){
            return -3;
        }
    }
    testWorld->getObjectByName("game_object")->setName("background");

     // sun two
    {
        float posSunTwo[2] = {0.6f, 0.6f};
        float iColorSunTwo[3] = {0.0f, 0.8f, 1.0f};
        float oColorSunTwo[3] = {0.8f, 0.0f, 1.0f};
        if( testWorld->addObject(
            redhand::createCircle(
                posSunTwo,
                0.6f,
                edges,
                iColorSunTwo,
                oColorSunTwo,
                testWorld->getShaderByName("default"),
                nullptr
            )
        ) < 0){
            return -3;
        }
    }
    testWorld->getObjectByName("game_object")->setName("sun2");

    // sun one
    float posSunOne[2] = {0.8f,0.45f};
    if( testWorld->addObject(redhand::createCircle(posSunOne, 0.35f, edges, NULL, NULL, testWorld->getShaderByName("default"), nullptr)) < 0){
        return -3;
    }
    testWorld->getObjectByName("game_object")->setName("sun1");

    //triangle
    {
        std::vector <float> trig_points = {0.0f,0.0f, 1.0f,0.0f, 0.5f,1.0f};
        std::vector <unsigned int> trig_indicies = {0,1,2};
        std::vector <float> trig_colors = {1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f};
        std::vector <float> trig_texels = {0.0f,0.0f, 1.0f*10.0f,0.0f, 0.5f*10.0f,1.0f*10.0f};
        std::vector <float> trig_pos = {-0.4f,-0.4f};
        if( testWorld->addObject(
            new redhand::game_object(
                trig_points,
                trig_indicies,
                trig_colors,
                testWorld->getShaderByName("default"),
                GL_DYNAMIC_DRAW,
                [](std::shared_ptr<redhand::shader>){},
                {0.5f,0.5f},
                0.0f,
                trig_pos,
                nullptr,
                trig_texels
            )
        ) < 0){
            return -3;
        }
        testWorld->getObjectByName("game_object")->setName("trig");

        testWorld->getObjectByName("trig")->setLoopFunction([](GLFWwindow*, redhand::game_object* obj){
            obj->setRotation((float)glfwGetTime()*20.0f);
        });
    }

    //house
    if( testWorld->addObject(redhand::createHouse(testWorld->getTextureByName("house"),testWorld->getShaderByName("default"),(800.0f*0.45)/testWorld->getTextureByName("house")->getWidth())) < 0){
        return -3;
    }
    testWorld->getObjectByName("game_object")->setName("house");

    testWorld->getObjectByName("house")->setLoopFunction(processHouseMovement);
    
    return 0;
}

void processHouseMovement(GLFWwindow* window, redhand::game_object* obj){
    //move the house
    std::vector<float> deltaPosition = {0.0f,0.0f};

    if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS){
        deltaPosition.at(0) = 0.002f;
    }else if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS){
        deltaPosition.at(0) = -0.002f;
    }

    if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS){
        deltaPosition.at(1) = 0.002f;
    }else if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS){
        deltaPosition.at(1) = -0.002f;
    }

    obj->move(deltaPosition);    

    //check for button presses and change rotation
    float deltaRotation = 0.0f;

    if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS){
        deltaRotation = -0.25f;
    }else if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){
        deltaRotation = 0.25f;
    }

    obj->rotate(deltaRotation);

}

void processWorldInput(GLFWwindow* window, std::shared_ptr<redhand::world> activeWorld){
    //move the camera
    std::vector<float> deltaCamera = {0.0f,0.0f};

    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        deltaCamera.at(0) = 0.002f;
    }else if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        deltaCamera.at(0) = -0.002f;
    }

    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        deltaCamera.at(1) = 0.002f;
    }else if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        deltaCamera.at(1) = -0.002f;
    }

    activeWorld->moveCamera(deltaCamera.at(0),deltaCamera.at(1));

}

void processGlobalInput(redhand::engine* game){

    auto window = game->getWindow();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        game->stopGame();
    }

    if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

std::unique_ptr<redhand::game_object> createHouse(
    std::shared_ptr<redhand::texture2D> texture,
    std::shared_ptr<redhand::shader> shade
){

    auto settings = redhand::DEFAULT_GAME_OBJECT_PROPERTIES;

    settings.attached_shader = shade;
    settings.attached_texture = texture;


    settings.points_coordinates = {
        {1.0f, 0.55f},  // top right
        {1.0f, 0.0f},  // bottom right
        {0.0f, 0.0f},  // bottom left
        {0.0f, 0.55f},  // top left 
        {0.5f, 1.0f}  // top middle
    };

    settings.triangle_indices = {
        {0, 1, 3},   // first triangle
        {1, 2, 3},    // second triangle
        {0, 3, 4}     //third triangle
    };

    settings.point_colors = {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f} 
    };

    settings.postition = {0.5f,0.5f};

    auto ptr = std::unique_ptr<redhand::game_object>(new redhand::game_object(settings));

    return ptr;
    
}


