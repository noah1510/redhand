#include "redhand/actor.hpp"
#include "redhand/event/game_loop_event.hpp"
#include "redhand/game_object.hpp"
#include "redhand/texture.hpp"
#include "redhand/types.hpp"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <future>
#include <memory>
#include <mutex>
#include <shared_mutex>

redhand::Actor::Actor():redhand::Actor(nullptr){}

redhand::Actor::Actor(redhand::texture2D* tex):redhand::game_object(){
    redhand::game_object_properties config;

    config.attached_shader = nullptr;

    image = std::shared_ptr<redhand::texture2D>(std::move(tex));
    config.attached_texture = image;

    config.scale = {1.0f,1.0f};
    if(image != nullptr){
        config.scale = {static_cast<float>(image->getWidth())/image->getHeight(),1.0f};
    }

    config.gl_drawing_mode = STATIC_DRAW;
    config.points_coordinates = {
        {1.0f, 0.0f}, //top left
        {1.0f, 1.0f}, //top right
        {0.0f, 1.0f}, //bottom right
        {0.0f, 0.0f}  //bottom left
    };

    config.triangle_indices = {
        {0, 1, 2}, // first triangle
        {0, 2, 3}  // second triangle
    };

    config.point_colors.resize(4);
    for (int i = 0; i < 4; i++) {
        config.point_colors.at(i) = {0.0f,0.0f,0.0f};
    }
    
    object_properties = config;
    updateBuffers();

    this->setColorAlpha(1.0f);

}

void redhand::Actor::onLoop(game_loop_event evt){
    this->act(evt);
}

float redhand::Actor::getX(){
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);
    return object_properties.postition.x;
}

float redhand::Actor::getY(){
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);
    return object_properties.postition.y;
}

void redhand::Actor::setActorScale(float factor){
    std::unique_lock<std::shared_mutex> lock(mutex_object_properties);
    object_properties.scale = {1.0f,1.0f};
    if(image != nullptr){
        object_properties.scale = {factor*static_cast<float>(image->getWidth())/image->getHeight(),factor};
    }
    lock.unlock();

    std::thread update([&](){
        std::scoped_lock<std::shared_mutex> lock2(mutex_actorScale);
        actorScale = factor;
    });

    updateWorldTransformation();

    update.join();

}

float redhand::Actor::getActorScale(){
    std::shared_lock<std::shared_mutex> lock2(mutex_actorScale);
    return actorScale;
}

glm::vec2 redhand::Actor::getSize(){
    std::shared_lock<std::shared_mutex> lock2(mutex_object_properties);
    return object_properties.scale;
}

void redhand::Actor::act(redhand::game_loop_event){}

void redhand::Actor::setImage(redhand::texture2D* img){
    std::unique_lock<std::shared_mutex> lock(mutex_image);
    image.reset();
    image = std::shared_ptr<redhand::texture2D>(std::move(img));

    std::unique_lock<std::shared_mutex> lock2(mutex_object_properties);
    object_properties.attached_texture = image;

    lock.unlock();
    lock2.unlock();

    scaleActor();
}

void redhand::Actor::scaleActor(float size){
    size *= getActorScale();
    setActorScale(size);
}

void redhand::Actor::setImage(std::filesystem::path img){
    redhand::image_properties prop;
    prop.file_location = img;
    auto* tex = new redhand::texture2D(prop);

    setImage(tex);
}

std::shared_ptr<redhand::texture2D> redhand::Actor::getImage(){
    std::shared_lock<std::shared_mutex> lock(mutex_image);
    return image;
}

float redhand::Actor::getRotation(){
    std::shared_lock<std::shared_mutex> lock(mutex_game_object);
    return object_properties.rotation;
}

glm::vec2 redhand::Actor::getPosition(){
    return {getX(),getY()};
}

void redhand::Actor::move(float distance){
    auto rot = getRotation();
    move({
        distance*rot*glm::cos(glm::radians(rot)),
        distance*rot*glm::sin(glm::radians(rot))
    });
}

void redhand::Actor::move(std::chrono::nanoseconds deltaT){
    float distance = movementSpeed * (deltaT/std::chrono::nanoseconds(std::chrono::seconds(1)));
    move(distance);
}

void redhand::Actor::move(glm::vec2 delta){
    mutex_object_properties.lock();

    object_properties.postition += delta;

    mutex_object_properties.unlock();

    updateWorldTransformation();
}

void redhand::Actor::setSpeed(float speed){
    std::scoped_lock<std::shared_mutex> lock(mutex_movementSpeed);
    movementSpeed = speed;
}

void redhand::Actor::turn(float delta){
    rotate(delta);
}

void redhand::Actor::turnTowards(glm::vec2 point){
    auto line = point - getPosition();
    setRotation(glm::degrees( glm::atan(line.y/line.x) ));
}