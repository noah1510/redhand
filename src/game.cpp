#include "game.hpp"

int game_init(
    world* startWorld
){
    int exitCode = 0;

    exitCode = createTestworld(startWorld);
    if(exitCode < 0){
        return exitCode;
    }

    return 0;   
}

int game_loop(
    GLFWwindow* window,
    world* activeWorld,
    world* nextWorld
){
    //process the input
    processGlobalInput(window);

    //clear the bg color
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int error = activeWorld->test();
    activeWorld->tick(window);

    //Update the window buffers
    glfwSwapBuffers(window);
    glfwPollEvents(); 

    nextWorld = nullptr;

    return 0;

}

int createTestworld(world* testWorld){
    //add shaders to world

    if( testWorld->addShader(new shader(
        "../shaders/default.vert",
        "../shaders/default.frag",
        "default"
        )
    ) == -1){
        return -1;
    }

    if(testWorld->getShaderByName("default") == nullptr){
        return -20;
    }

    //Add textures to world
    if(testWorld->addTexture(
        new texture2D(
            "../textures/open/crate.png"
        )) == -1){
        return -2;
    }

    if(testWorld->addTexture(
        new texture2D(
            "../textures/open/house.png"
        )) == -1){
        return -2;
    }
    
    //creating the objects and add them to the world
    int edges = 240;

     // Background rectangle
    {
        float bottomLeft[2] = {-1.0f, -1.0f};
        float color[3] = {0.0f, 0.6f, 1.0f};
        if( testWorld->addObject(
            createRecktangle(
                bottomLeft,
                2.0f,
                2.0f,
                color,
                testWorld->getShaderByName("default"),
                testWorld->getTextureByIndex(1),
                GL_STATIC_DRAW,
                100.0f
            )
        ) == -1){
            return -3;
        }
    }

     // sun two
    {
        float posSunTwo[2] = {0.6f, 0.6f};
        float iColorSunTwo[3] = {0.0f, 0.8f, 1.0f};
        float oColorSunTwo[3] = {0.8f, 0.0f, 1.0f};
        if( testWorld->addObject(
            createCircle(
                posSunTwo,
                0.6f,
                edges,
                iColorSunTwo,
                oColorSunTwo,
                testWorld->getShaderByName("default"),
                nullptr
            )
        ) == -1){
            return -3;
        }
    }

    // sun one
    float posSunOne[2] = {0.8f,0.45f};
    if( testWorld->addObject(createCircle(posSunOne, 0.35f, edges, NULL, NULL, testWorld->getShaderByName("default"), nullptr)) == -1){
        return -3;
    }

    //triangle
    {
        std::vector <float> trig_points = {0.0f,0.0f, 1.0f,0.0f, 0.5f,1.0f};
        std::vector <unsigned int> trig_indicies = {0,1,2};
        std::vector <float> trig_colors = {1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f};
        std::vector <float> trig_texels = {0.0f,0.0f, 1.0f*10.0f,0.0f, 0.5f*10.0f,1.0f*10.0f};
        std::vector <float> trig_pos = {-0.4f,-0.4f};
        if( testWorld->addObject(
            new object(
                trig_points,
                trig_indicies,
                trig_colors,
                testWorld->getShaderByName("default"),
                GL_DYNAMIC_DRAW,
                [](shader* shade){},
                {0.5f,0.5f},
                0.0f,
                trig_pos,
                nullptr,
                trig_texels
            )
        ) == -1){
            return -3;
        }

        testWorld->getObjectByIndex(3)->setLoopFunction([](GLFWwindow* window, object* obj){
            obj->setRotation((float)glfwGetTime()*20.0f);
        });
    }

    //house
    if( testWorld->addObject(createHouse(testWorld->getTextureByIndex(0),testWorld->getShaderByName("default"),100.0f)) == -1){
        return -3;
    }

    testWorld->getObjectByIndex(4)->setLoopFunction(processHouseMovement);

    return testWorld->test();
}

void processHouseMovement(GLFWwindow* window, object* obj){
    //check for button presses and change position
    std::vector<float> deltaPosition = {0.0f,0.0f};

    if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        deltaPosition.at(0) = 0.02f;
    }else if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        deltaPosition.at(0) = -0.02f;
    }

    if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        deltaPosition.at(1) = 0.02f;
    }else if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        deltaPosition.at(1) = -0.02f;
    }

    obj->move(deltaPosition);

    //check for button presses and change rotation
    float deltaRotation = 0.0f;

    if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS){
        deltaRotation = -2.5f;
    }else if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){
        deltaRotation = 2.5f;
    }

    obj->rotate(deltaRotation);

}

void processGlobalInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

