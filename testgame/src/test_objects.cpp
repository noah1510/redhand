#include "test_objects.hpp"

house::house(
    std::shared_ptr<redhand::texture2D> texture
){

    redhand::game_object_properties settings;

    settings.attached_shader = nullptr;
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

    object_properties = settings;
    updateBuffers();

}

void house::onLoop(redhand::game_loop_event){
    //move the house

    if( redhand::input_system::static_isKeyPressed(redhand::KEY_RIGHT) ){
        this->move({0.02f,0.0f});
    }else if(redhand::input_system::static_isKeyPressed(redhand::KEY_LEFT)){
        this->move({-0.02f,0.0f});
    }

    if(redhand::input_system::static_isKeyPressed(redhand::KEY_UP)){
        this->move({0.0f,0.02f});
    }else if(redhand::input_system::static_isKeyPressed(redhand::KEY_DOWN)){
        this->move({0.0f,-0.02f});
    } 

    //check for button presses and change rotation

    if(redhand::input_system::static_isKeyPressed(redhand::KEY_E)){
        this->rotate(-2.5f);
    }else if(redhand::input_system::static_isKeyPressed(redhand::KEY_Q)){
        this->rotate(2.5f);
    }

    return;

}


hand::hand(
    std::shared_ptr<redhand::texture2D> texture
){

    redhand::game_object_properties settings;

    settings.attached_shader = nullptr;
    settings.attached_texture = texture;
    settings.name = "hand";

    settings.points_coordinates = {
        {1.0f, 1.0f},  // top right
        {1.0f, 0.0f},  // bottom right
        {0.0f, 0.0f},  // bottom left
        {0.0f, 1.0f}  // top left 
    };

    settings.triangle_indices = {
        {0, 1, 3},   // first triangle
        {1, 2, 3}    // second triangle
    };

    settings.point_colors = {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };

    settings.postition = {-0.5f, +0.5f};
    settings.scale = {0.5f * static_cast<float>(texture->getWidth())/texture->getHeight(),0.5f};

    object_properties = settings;
    updateBuffers();

    this->setColorAlpha(1.0f);

}

void hand::onLoop(redhand::game_loop_event evt){
    //move the house
    auto delta = evt.getFrameTime().count();
    this->rotate(static_cast<float>(delta) * 30.0f / 1000000000.0f );

    return;
}

void hand::updateWorldTransformation() {
    auto lock1 = std::shared_lock(mutex_object_properties);
    auto local_object_properties = object_properties;
    lock1.unlock();

    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(
        scale, 
        glm::vec3(local_object_properties.scale, 1.0f)
    );
    auto delta = scale * glm::vec4(local_object_properties.rotation_point,0.0f,1.0f);


    glm::mat4 local_world_transformation = glm::mat4(1.0f);
    local_world_transformation = glm::translate(
        local_world_transformation, 
        glm::vec3(local_object_properties.postition, 0.0f)
    );

    local_world_transformation = glm::translate(
        local_world_transformation, 
        glm::vec3(delta)
    );

    local_world_transformation = glm::rotate(
        local_world_transformation, 
        glm::radians(local_object_properties.rotation), 
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    local_world_transformation = glm::translate(
        local_world_transformation, 
        glm::vec3(-delta)
    );

    local_world_transformation = glm::scale(
        local_world_transformation, 
        glm::vec3(local_object_properties.scale, 1.0f)
    );


    auto lock2 = std::scoped_lock(mutex_world_transformation);
    world_transformation = local_world_transformation;
}
