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

redhand::Actor::Actor() : redhand::Actor(nullptr) {}

redhand::Actor::Actor(redhand::texture2D *tex) : redhand::game_object() {
    redhand::game_object_properties config;

    config.attached_shader = nullptr;

    image = std::shared_ptr<redhand::texture2D>(std::move(tex));
    config.attached_texture = image;

    config.scale = {1.0f, 1.0f};
    if (image != nullptr) {
        config.scale = {static_cast<float>(image->getWidth()) / image->getHeight(), 1.0f};
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
        config.point_colors.at(i) = {0.0f, 0.0f, 0.0f};
    }

    config.texture_coordinates = {
        {1.0f, 0.0f}, //top left
        {1.0f, 1.0f}, //top right
        {0.0f, 1.0f}, //bottom right
        {0.0f, 0.0f}  //bottom left
    };

    object_properties = config;
    updateBuffers();

    this->setColorAlpha(1.0f);
}

void redhand::Actor::setName(std::string name) {
    std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);

    object_properties.name = name;
}

std::string_view redhand::Actor::getName() {
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.name;
}

void redhand::Actor::onLoop(game_loop_event evt) {
    this->act(evt);
}

float redhand::Actor::getX() {
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);
    return object_properties.postition.x;
}

float redhand::Actor::getY() {
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);
    return object_properties.postition.y;
}

void redhand::Actor::setActorScale(float factor) {
    std::unique_lock<std::shared_mutex> lock(mutex_object_properties);
    object_properties.scale = {1.0f, 1.0f};
    if (image != nullptr) {
        object_properties.scale = {factor * static_cast<float>(image->getWidth()) / image->getHeight(), factor};
    }
    lock.unlock();

    std::thread update([&]() {
        std::scoped_lock<std::shared_mutex> lock2(mutex_actorScale);
        actorScale = factor;
    });

    updateWorldTransformation();

    update.join();
}

float redhand::Actor::getActorScale() {
    std::shared_lock<std::shared_mutex> lock2(mutex_actorScale);
    return actorScale;
}

glm::vec2 redhand::Actor::getSize() {
    std::shared_lock<std::shared_mutex> lock2(mutex_object_properties);
    return object_properties.scale;
}

void redhand::Actor::act(redhand::game_loop_event) {}

void redhand::Actor::setImage(std::shared_ptr<redhand::texture2D> img) {
    std::unique_lock<std::shared_mutex> lock(mutex_image);
    if (img == image) {
        return;
    }
    image.reset();

    if (img != nullptr && img->hasErrord()) {
        img = nullptr;
        std::cerr << "the given image had an error" << std::endl;
    }
    image = img;

    std::unique_lock<std::shared_mutex> lock2(mutex_object_properties);
    object_properties.attached_texture = image;
    lock2.unlock();

    if (image != nullptr) {
        std::scoped_lock<std::shared_mutex> lock3(mutex_texture_mode);
        texture_mode = 1;
    } else {
        std::scoped_lock<std::shared_mutex> lock3(mutex_texture_mode);
        texture_mode = 0;
    }

    lock.unlock();

    scaleActor();
}

void redhand::Actor::scaleActor(float size) {
    size *= getActorScale();
    setActorScale(size);
}

void redhand::Actor::setImage(std::filesystem::path img) {
    redhand::image_properties prop;
    prop.file_location = img;
    setImage(prop);
}

void redhand::Actor::setImage(redhand::image_properties img) {
    auto tex = std::make_shared<redhand::texture2D>(img);
    setImage(tex);
}

std::shared_ptr<redhand::texture2D> redhand::Actor::getImage() {
    std::shared_lock<std::shared_mutex> lock(mutex_image);
    return image;
}

float redhand::Actor::getRotation() {
    std::shared_lock<std::shared_mutex> lock(mutex_game_object);
    return object_properties.rotation;
}

glm::vec2 redhand::Actor::getPosition() {
    return {getX(), getY()};
}

void redhand::Actor::move(float distance) {
    auto rot = getRotation();
    move({distance * rot * glm::cos(glm::radians(rot)),
          distance * rot * glm::sin(glm::radians(rot))});
}

void redhand::Actor::move(std::chrono::nanoseconds deltaT) {
    float distance = movementSpeed * (deltaT / std::chrono::nanoseconds(std::chrono::seconds(1)));
    move(distance);
}

void redhand::Actor::move(glm::vec2 delta) {
    mutex_object_properties.lock();

    object_properties.postition += delta;

    mutex_object_properties.unlock();

    updateWorldTransformation();
}

void redhand::Actor::setSpeed(float speed) {
    std::scoped_lock<std::shared_mutex> lock(mutex_movementSpeed);
    movementSpeed = speed;
}

void redhand::Actor::turn(float delta) {
    rotate(delta);
}

void redhand::Actor::turnTowards(glm::vec2 point) {
    auto line = point - getPosition();
    setRotation(glm::degrees(glm::atan(line.y / line.x)));
}

void redhand::Actor::setPosition(glm::vec2 location) {
    mutex_object_properties.lock();

    object_properties.postition = location;

    mutex_object_properties.unlock();

    updateWorldTransformation();
}

void redhand::Actor::updateBufferData(std::vector<glm::vec2> points, std::vector<std::array<unsigned int, 3>> indicies, std::vector<glm::vec3> colors) {
    mutex_object_properties.lock();

    object_properties.points_coordinates = points;
    object_properties.triangle_indices = indicies;
    object_properties.point_colors = {};
    if (points.size() == colors.size()) {
        object_properties.point_colors = colors;
    }

    mutex_object_properties.unlock();

    updateBuffers();
}

void redhand::Actor::setColorAlpha(float alpha) {
    if (alpha >= 0.0f && alpha <= 1.0f) {
        std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);
        object_properties.alpha_value = alpha;
    }
}

void redhand::Actor::setRotaionAxis(glm::vec3 axis) {
    mutex_object_properties.lock();

    object_properties.rotation_axis = axis;

    mutex_object_properties.unlock();

    updateWorldTransformation();
}

std::vector<glm::vec2> redhand::Actor::getHitbox(){
    auto pos = getPosition();
    auto size = getSize();

    std::vector<glm::vec2> ret = {pos, pos+glm::vec2{size.x,0},pos+size,pos+glm::vec2{0,size.y}};

    std::shared_lock<std::shared_mutex> lock1(mutex_world_transformation);

    for (unsigned int i=0;i<ret.size();i++){
        ret.at(i) = world_transformation * glm::vec4( ret.at(i),0,0);
    }

    return ret;
}