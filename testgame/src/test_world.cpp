#include "test_world.hpp"

int test_world::onCreate(redhand::event<redhand::engine>){
    auto shader1 = std::unique_ptr<redhand::shader>(new redhand::shader());
    if( this->addShader(std::move(shader1)) < 0){
        std::cerr << "Got error while adding shader" << std::endl;
        return -10;
    }

    if(this->getShaderByName("default") == nullptr){
        std::cerr << "Got nullpointer as shader" << std::endl;
        return -11;
    }

    //Add textures to world
    auto tex0 = std::unique_ptr<redhand::texture2D>(new redhand::texture2D("textures/open/crate.png","house"));
    if(this->addTexture(std::move(tex0)) < 0){
        return -20;
    }

    auto tex1 = std::unique_ptr<redhand::texture2D>(new redhand::texture2D("textures/open/house.png","bg"));
    if(this->addTexture(std::move(tex1)) < 0){
        return -21;
    }

    auto tex2 = std::unique_ptr<redhand::texture2D>(new redhand::texture2D("logo/redhand.svg.png","hand"));
    if(this->addTexture(std::move(tex2)) < 0){
        return -22;
    }
    
    //creating the objects and add them to the world
    int edges = 240;

    // Background rectangle 
    if( this->addObject(
        redhand::createRectangle(
            {-100.0f, -100.0f},
            200.0f,
            200.0f,
            {0.0f, 0.6f, 1.0f},
            this->getShaderByName("default"),
            this->getTextureByName("bg"),
            redhand::STATIC_DRAW,
            "background",
            -1.0f
        )
    ) < 0){
        return -3;
    }

     // sun two
    if( this->addObject(
        redhand::createCircle(
            {0.6f, 0.6f},
            0.6f,
            edges,
            {0.0f, 0.8f, 1.0f},
            {0.8f, 0.0f, 1.0f},
            this->getShaderByName("default"),
            nullptr,
            "sun2"
        )
    ) < 0){
        return -3;
    }

    // sun one
    if( this->addObject(redhand::createCircle({0.8f,0.45f}, 0.35f, edges, {1.0f,1.0f,0.0f}, {1.0f,0.5f,0.0f}, this->getShaderByName("default"), nullptr, "sun1")) < 0){
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
    trig_properties.attached_shader = this->getShaderByName("default");
    trig_properties.scale = {0.5f,0.5f};
    trig_properties.rotation_point = {0.0f,0.0f};
    
    if( this->addObject(std::unique_ptr<redhand::game_object>(new redhand::game_object(trig_properties)) ) < 0){
        return -3;
    }    

    //house
    if( this->addObject(
        std::move( std::unique_ptr<redhand::game_object>( new house( this->getTextureByName("house"), this->getShaderByName("default"))))
    ) < 0){
        return -3;
    }

    //hand
    if( this->addObject(
        std::move( std::unique_ptr<redhand::game_object>( new hand( this->getTextureByName("hand"), this->getShaderByName("default"))))
    ) < 0){
        return -3;
    }
    
    return 0;
}

void test_world::tick(redhand::game_loop_event evt) {
    complex_world::tick(evt);

    this->getObjectByName("trig")->rotate(1.0f);

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

    this->moveCamera(deltaCamera.at(0),deltaCamera.at(1));

    return;
}