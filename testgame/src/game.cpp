#include "game.hpp"

int game_init(
    std::shared_ptr<redhand::complex_world> startWorld
){
    int exitCode = 0;

    exitCode = createTestworld(startWorld);
    if(exitCode < 0){
        return exitCode;
    }

    return 0;   
}

int main_game_logic(
    redhand::game_loop_event
){


    return 0;

}

int createTestworld(std::shared_ptr<redhand::complex_world> testWorld){
    //add shaders to world

    auto shader1 = std::unique_ptr<redhand::shader>(new redhand::shader());
    if( testWorld->addShader(std::move(shader1)) < 0){
        std::cerr << "Got error while adding shader" << std::endl;
        return -10;
    }

    if(testWorld->getShaderByName("default") == nullptr){
        std::cerr << "Got nullpointer as shader" << std::endl;
        return -11;
    }

    //Add textures to world
    auto tex0 = std::unique_ptr<redhand::texture2D>(new redhand::texture2D("textures/open/crate.png","house"));
    if(testWorld->addTexture(std::move(tex0)) < 0){
        return -20;
    }

    auto tex1 = std::unique_ptr<redhand::texture2D>(new redhand::texture2D("textures/open/house.png","bg"));
    if(testWorld->addTexture(std::move(tex1)) < 0){
        return -21;
    }
    
    //creating the objects and add them to the world
    int edges = 240;

    // Background rectangle 
    if( testWorld->addObject(
        redhand::createRectangle(
            {-100.0f, -100.0f},
            200.0f,
            200.0f,
            {0.0f, 0.6f, 1.0f},
            testWorld->getShaderByName("default"),
            testWorld->getTextureByName("bg"),
            GL_STATIC_DRAW,
            "background",
            -1.0f
        )
    ) < 0){
        return -3;
    }

     // sun two
    if( testWorld->addObject(
        redhand::createCircle(
            {0.6f, 0.6f},
            0.6f,
            edges,
            {0.0f, 0.8f, 1.0f},
            {0.8f, 0.0f, 1.0f},
            testWorld->getShaderByName("default"),
            nullptr,
            "sun2"
        )
    ) < 0){
        return -3;
    }

    // sun one
    if( testWorld->addObject(redhand::createCircle({0.8f,0.45f}, 0.35f, edges, {1.0f,1.0f,0.0f}, {1.0f,0.5f,0.0f}, testWorld->getShaderByName("default"), nullptr, "sun1")) < 0){
        return -3;
    }

    //triangle
    auto trig_properties = redhand::DEFAULT_GAME_OBJECT_PROPERTIES;
    trig_properties.points_coordinates = {{0.0f,0.0f}, {1.0f,0.0f}, {0.5f,1.0f}};
    trig_properties.triangle_indices = {{0,1,2}};
    trig_properties.point_colors = {{1.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f}, {0.0f,0.0f,1.0f}};
    trig_properties.texture_coordinates = {{0.0f,0.0f}, {1.0f*10.0f,0.0f}, {0.5f*10.0f,1.0f*10.0f}};
    trig_properties.postition = {-0.4f,-0.4f};
    trig_properties.name = "trig";
    trig_properties.attached_shader = testWorld->getShaderByName("default");
    trig_properties.scale = {0.5f,0.5f};
    trig_properties.rotation_point = {0.0f,0.0f};
    
    if( testWorld->addObject(std::unique_ptr<redhand::game_object>(new redhand::game_object(trig_properties)) ) < 0){
        return -3;
    }

    testWorld->getObjectByName("trig")->setLoopFunction([](GLFWwindow*, redhand::game_object* obj){
        obj->rotate(1.0f);
    });
    

    //house
    if( testWorld->addObject(
        createHouse(testWorld->getTextureByName("house"), testWorld->getShaderByName("default"))
    ) < 0){
        return -3;
    }

    auto main = redhand::input_system::getEngine();
    if (main != nullptr){
        main->addGameLoopHandler(processHouseMovement,"house");
    }
    
    return 0;
}

int processHouseMovement(redhand::game_loop_event evt){
    //move the house
    auto ctrl = evt.getRaiser();
    auto house = ctrl->getActiveWorld()->getObjectByName("house");

    if(house == nullptr){
        return 0;
    }

    if( redhand::input_system::static_isKeyPressed(redhand::KEY_RIGHT) ){
        house->move({0.02f,0.0f});
    }else if(redhand::input_system::static_isKeyPressed(redhand::KEY_LEFT)){
        house->move({-0.02f,0.0f});
    }

    if(redhand::input_system::static_isKeyPressed(redhand::KEY_UP)){
        house->move({0.0f,0.02f});
    }else if(redhand::input_system::static_isKeyPressed(redhand::KEY_DOWN)){
        house->move({0.0f,-0.02f});
    } 

    //check for button presses and change rotation

    if(redhand::input_system::static_isKeyPressed(redhand::KEY_E)){
        house->rotate(-2.5f);
    }else if(redhand::input_system::static_isKeyPressed(redhand::KEY_Q)){
        house->rotate(2.5f);
    }

    return 0;

}

int processWorldInput(redhand::game_loop_event evt){

    auto activeWorld = evt.getRaiser()->getActiveWorld();
    if(activeWorld == nullptr){
        return 0;
    }

    //move the camera
    std::array<float,2> deltaCamera = {0.0f,0.0f};

    if( redhand::input_system::static_isKeyPressed(redhand::KEY_D) ){
        deltaCamera.at(0) = 0.02f;
    }else if( redhand::input_system::static_isKeyPressed(redhand::KEY_A) ){
        deltaCamera.at(0) = -0.02f;
    }

    if( redhand::input_system::static_isKeyPressed(redhand::KEY_W) ){
        deltaCamera.at(1) = 0.02f;
    }else if( redhand::input_system::static_isKeyPressed(redhand::KEY_S) ){
        deltaCamera.at(1) = -0.02f;
    }

    activeWorld->moveCamera(deltaCamera.at(0),deltaCamera.at(1));

    return 0;

}

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

std::unique_ptr<redhand::game_object> createHouse(
    std::shared_ptr<redhand::texture2D> texture,
    std::shared_ptr<redhand::shader> shade
){

    auto settings = redhand::DEFAULT_GAME_OBJECT_PROPERTIES;

    settings.attached_shader = shade;
    settings.attached_texture = texture;
    settings.name = "house";

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

    settings.postition = {-0.1f,-0.1f};
    settings.scale = {0.5f,0.5f};

    auto ptr = std::unique_ptr<redhand::game_object>(new redhand::game_object(settings));

    return std::move(ptr);
    
}


