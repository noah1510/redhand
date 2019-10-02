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
    int ret = testWorld->addShader(new shader(
        "../shaders/minimalColorTexture.vert",
        "../shaders/texture.frag"
        )
    );
    if(ret == -1){
        return -1;
    }

    testWorld->addShader(new shader(
        "../shaders/advanced1.vert",
        "../shaders/variableColor.frag"
        )
    );
    if(ret == -1){
        return -1;
    }

    testWorld->addShader(new shader(
        "../shaders/advanced2.vert",
        "../shaders/texture.frag"
        )
    );
    if(ret == -1){
        return -1;
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
        float topRight[3] = {1.0f, 1.0f, 0.0f};
        float bottomLeft[3] = {-1.0f, -1.0f, 0.0f};
        float colorTopRight[3] = {1.0f, 1.0f, 1.0f};
        float colorBottomLeft[3] = {0.0f, 0.0f, 0.0f};
        float texTopRight[2] = {10.0f, 10.0f};
        float texBottomLeft[2] = {0.0f, 0.0f};
        ret = testWorld->addObject(
            createColorTextureRectangle(
                topRight,
                bottomLeft,
                colorTopRight,
                colorBottomLeft,
                texTopRight,
                texBottomLeft,
                testWorld->getShader(0),
                testWorld->getTexture(0),
                GL_STATIC_DRAW
            )
        );
        if(ret == -1){
            return -3;
        }
    }

     // sun two
    {
        float posSunTwo[3] = {0.3f, 0.7f, 0.0f};
        float iColorSunTwo[3] = {0.0f, 0.8f, 1.0f};
        float oColorSunTwo[3] = {0.8f, 0.0f, 1.0f};
        ret = testWorld->addObject(
            createCircle(
                posSunTwo,
                0.35f,
                edges,
                iColorSunTwo,
                oColorSunTwo,
                testWorld->getShader(0),
                nullptr
            )
        );
        if(ret == -1){
            return -3;
        }
    }

    // sun one
    ret = testWorld->addObject(createCircle(NULL, 0.25f, edges, NULL, NULL, testWorld->getShader(0), nullptr));
    if(ret == -1){
        return -3;
    }

    //triangle
    {
        std::vector <float> trig_points = {0.0f,0.5f,0.0f, -0.25f,0.0f,0.0f, 0.25f,0.0f,0.0f};
        std::vector <unsigned int> trig_indicies = {0,1,2};
        std::vector <float> trig_colors = {1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f};
        std::vector <float> trig_texels = {5.0f,10.0f, 0.0f,0.0f, 10.0f,0.0f};
        ret = testWorld->addObject(
            new object(
                trig_points,
                trig_indicies,
                trig_colors,
                trig_texels,
                testWorld->getShader(2),
                nullptr,
                GL_STATIC_DRAW,
                [](shader* shade){
                    glm::mat4 trans = glm::mat4(1.0f);
                    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
                    trans = glm::translate(trans, glm::vec3(0.25f, 0.25f, 0.0f));

                    unsigned int transformLoc = glGetUniformLocation(shade->ID, "transformation");
                    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
                }
            )
        );
        if(ret == -1){
            return -3;
        }

        testWorld->getObject(3)->setLoopFunction([](GLFWwindow* window, object* obj){
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

            unsigned int transformLoc = glGetUniformLocation(obj->getShader()->ID, "transformation");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        });
    }

    //house
    ret = testWorld->addObject(createHouse(testWorld->getTexture(1),testWorld->getShader(1)));
    if(ret == -1){
        return -3;
    }

    testWorld->getObject(4)->setLoopFunction(processHouseMovement);

    return testWorld->test();
}

void processHouseMovement(GLFWwindow* window, object* obj){
    shader* shade = obj->getShader();
    float currentOffset[4];
    shade->getFloat("positionOffset", currentOffset);

    if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS){
        currentOffset[0] += 0.02f;
    }else if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS){
        currentOffset[0] -= 0.02f;
    }

    if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS){
        currentOffset[1] += 0.02f;
    }else if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS){
        currentOffset[1] -= 0.02f;
    }

    currentOffset[2] = 0.0f;

    shade->setFloat("positionOffset", currentOffset[0], currentOffset[1], currentOffset[2], currentOffset[3]);
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

