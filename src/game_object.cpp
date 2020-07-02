#include <redhand/glad/glad.h>

#include "redhand/game_object.hpp"
#include "redhand/shader.hpp"
#include "redhand/texture.hpp"

using namespace redhand;

bool redhand::game_object::checkObjectProperties(game_object_properties properties) {
    std::shared_mutex errorMutex;
    bool Error = false;

    if (properties.triangle_indices.size() == 0) {
        std::cerr << "ERROR::REDHAND::GAME_OBJECT::NO_TRIANGLES" << std::endl;
        Error = true;
    }

    auto less = std::async(std::launch::async, [&]() {
        for (unsigned int i = 0; i < properties.points_coordinates.size(); i++) {
            if (properties.points_coordinates.at(i).x < 0.0f || properties.points_coordinates.at(i).y < 0.0f) {
                std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
                auto lock = std::scoped_lock(errorMutex);
                Error = true;
                break;
            }

            auto lock = std::shared_lock(errorMutex);
            if (Error) {
                break;
            };
        }
    });

    auto more = std::async(std::launch::async, [&]() {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            if (properties.points_coordinates.at(i).x > 1.0f || properties.points_coordinates.at(i).y > 1.0f) {
                std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
                auto lock = std::scoped_lock(errorMutex);
                Error = true;
                break;
            }

            auto lock = std::shared_lock(errorMutex);
            if (Error) {
                break;
            };
        }
    });

    less.wait();
    more.wait();

    return Error;
}

void redhand::game_object::updateBuffers() {

    //check the object properties for an error
    if (checkObjectProperties(object_properties)) {
        return;
    }

    //lock the object properties
    auto lock = std::shared_lock(mutex_object_properties);

    //Initilize the buffers
    if (!glad_glIsVertexArray(VAO)) {
        glGenVertexArrays(1, &VAO);
    };
    if (!glad_glIsBuffer(VBO)) {
        glGenBuffers(1, &VBO);
    };
    if (!glad_glIsBuffer(EBO)) {
        glGenBuffers(1, &EBO);
    };

    //Reserve the needed size in the vecctor
    data.reserve(
        object_properties.points_coordinates.size() * 3   //size of points
        + object_properties.points_coordinates.size() * 3 //size of colors
        + object_properties.points_coordinates.size() * 2 //size of texels
    );

    //copy everything to the data vector
    for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
        data.push_back(object_properties.points_coordinates.at(i).x);
        data.push_back(object_properties.points_coordinates.at(i).y);
        data.push_back(0.0f);
    }

    if (object_properties.point_colors.size() != object_properties.points_coordinates.size()) {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
        }
    } else {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            data.push_back(object_properties.point_colors.at(i).r);
            data.push_back(object_properties.point_colors.at(i).g);
            data.push_back(object_properties.point_colors.at(i).b);
        }
    }

    if (object_properties.texture_coordinates.size() == object_properties.points_coordinates.size()) {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            data.push_back(object_properties.texture_coordinates.at(i).x);
            data.push_back(object_properties.texture_coordinates.at(i).y);
        };
    } else {
        for (unsigned int i = 0; i < object_properties.points_coordinates.size(); i++) {
            data.push_back(object_properties.points_coordinates.at(i).x);
            data.push_back(object_properties.points_coordinates.at(i).y);
        }
    }

    //create the vector containing the indicies
    std::vector<unsigned int> trigs;
    trigs.reserve(object_properties.triangle_indices.size() * 3);
    for (unsigned int i = 0; i < object_properties.triangle_indices.size(); i++) {
        trigs.push_back(object_properties.triangle_indices.at(i).at(0));
        trigs.push_back(object_properties.triangle_indices.at(i).at(1));
        trigs.push_back(object_properties.triangle_indices.at(i).at(2));
    }

    ///Create arrays and buffers
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), object_properties.gl_drawing_mode);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), object_properties.gl_drawing_mode);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * trigs.size(), trigs.data(), object_properties.gl_drawing_mode);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(object_properties.points_coordinates.size() * 3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)((object_properties.points_coordinates.size() * 6) * sizeof(float)));
    glEnableVertexAttribArray(2);

    if (object_properties.attached_texture != NULL && object_properties.attached_texture != nullptr && !object_properties.attached_texture->hasErrord()) {
        texture_mode = 1;
    };

    lock.unlock();

    updateWorldTransformation();
}

redhand::game_object::game_object() {}

//full constructor
redhand::game_object::game_object(game_object_properties properties) {
    object_properties = properties;

    updateBuffers();
}

redhand::game_object::~game_object() {
    triggerError();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    auto lock = std::scoped_lock(
        mutex_game_object,
        mutex_has_errored,
        mutex_object_properties,
        mutex_texture_scale,
        mutex_world_transformation,
        mutex_data,
        mutex_texture_mode);

    try {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void redhand::game_object::setScreenSize(int, int) {
    auto lock1 = std::shared_lock(mutex_object_properties);
    if (object_properties.attached_texture == nullptr || object_properties.attached_texture == NULL) {
        return;
    }
    auto local_object_properties = object_properties;
    lock1.unlock();

    auto lock2 = std::shared_lock(mutex_texture_mode);
    if (texture_mode == 1) {
        auto lock3 = std::scoped_lock(mutex_texture_scale);
        if (object_properties.automatic_scaling) {
            texture_scale.x = local_object_properties.texture_scale.x * local_object_properties.scale.x;
            texture_scale.y = local_object_properties.texture_scale.y * local_object_properties.scale.y;
        } else {
            texture_scale.x = local_object_properties.texture_scale.x;
            texture_scale.y = local_object_properties.texture_scale.y;
        }
    }
}

void redhand::game_object::draw(redhand::drawing_event evt) {
    auto lock1 = std::shared_lock(mutex_object_properties);
    auto local_object_properties = object_properties;
    if (object_properties.attached_shader == nullptr) {
        local_object_properties.attached_shader = evt.getDefaultShader();
    }
    lock1.unlock();

    //if there are textures set the texture scale of the shader
    auto lock2 = std::shared_lock(mutex_texture_mode);
    if (texture_mode == 1) {
        local_object_properties.attached_shader->setTextureScale(texture_scale);
    }
    auto local_texture_mode = texture_mode;
    lock2.unlock();

    //enable texture shader
    local_object_properties.attached_shader->use();

    //set the uniform variables
    local_object_properties.attached_shader->setInt("textureMode", texture_mode);
    local_object_properties.attached_shader->setFloat("colorAlpha", local_object_properties.alpha_value);

    //bind texture and draw background
    if (local_texture_mode == 1) {
        local_object_properties.attached_texture->bind(0);
    }

    //set World transformation matrix
    auto lock3 = std::shared_lock(mutex_world_transformation);
    unsigned int transformLoc = glGetUniformLocation(local_object_properties.attached_shader->getID(), "worldTransformation");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(world_transformation));
    lock3.unlock();

    //actually draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, local_object_properties.triangle_indices.size() * 3, GL_UNSIGNED_INT, 0);

    //reset the uniform values just in case
    local_object_properties.attached_shader->setInt("textureMode", 0);
    local_object_properties.attached_shader->setFloat("colorAlpha", 1.0f);
}

void redhand::game_object::updateWorldTransformation() {
    auto lock1 = std::shared_lock(mutex_object_properties);
    auto local_object_properties = object_properties;
    lock1.unlock();

    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(
        scale,
        glm::vec3(local_object_properties.scale, 1.0f));
    auto delta = scale * glm::vec4(local_object_properties.rotation_point, 0.0f, 1.0f);

    glm::mat4 local_world_transformation = glm::mat4(1.0f);
    local_world_transformation = glm::translate(
        local_world_transformation,
        glm::vec3(local_object_properties.postition, 0.0f));

    local_world_transformation = glm::translate(
        local_world_transformation,
        glm::vec3(delta));

    local_world_transformation = glm::rotate(
        local_world_transformation,
        glm::radians(local_object_properties.rotation),
        glm::vec3(0.0f, 0.0f, 1.0f));

    local_world_transformation = glm::translate(
        local_world_transformation,
        glm::vec3(-delta));

    local_world_transformation = glm::scale(
        local_world_transformation,
        glm::vec3(local_object_properties.scale, 1.0f));

    auto lock2 = std::scoped_lock(mutex_world_transformation);
    world_transformation = local_world_transformation;
}

bool redhand::game_object::hasErrord() {
    std::shared_lock<std::shared_mutex> lock(mutex_has_errored);

    return has_errored;
}

void redhand::game_object::triggerError() {
    std::scoped_lock<std::shared_mutex> lock(mutex_has_errored);

    has_errored = true;
}

void redhand::game_object::setColorAlpha(float alpha) {
    if (alpha >= 0.0f && alpha <= 1.0f) {
        std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);
        object_properties.alpha_value = alpha;
    }
}

void redhand::game_object::onLoop(redhand::game_loop_event){

};

std::shared_ptr<redhand::shader> redhand::game_object::getShader() {
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return std::shared_ptr<redhand::shader>(object_properties.attached_shader);
};

glm::vec2 redhand::game_object::getPosition() {
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.postition;
};

void redhand::game_object::setPosition(glm::vec2 pos) {
    mutex_object_properties.lock();

    object_properties.postition = pos;

    mutex_object_properties.unlock();

    updateWorldTransformation();
};

void redhand::game_object::move(glm::vec2 delta_pos) {
    mutex_object_properties.lock();

    object_properties.postition += delta_pos;

    mutex_object_properties.unlock();

    updateWorldTransformation();
};

float redhand::game_object::getRotation() {
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.rotation;
};

glm::vec2 redhand::game_object::getScale() {
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.scale;
};

void redhand::game_object::setRotation(float rot) {
    while (rot >= 360.0f) {
        rot -= 360.0f;
    }
    while (rot < 0.0f) {
        rot += 360.0f;
    }

    mutex_object_properties.lock();

    object_properties.rotation = rot;

    mutex_object_properties.unlock();

    updateWorldTransformation();
};

void redhand::game_object::rotate(float delta_rot) {

    while (delta_rot >= 360.0f) {
        delta_rot -= 360.0f;
    }
    while (delta_rot < 0.0f) {
        delta_rot += 360.0f;
    }

    mutex_object_properties.lock();

    object_properties.rotation += delta_rot;

    if (object_properties.rotation >= 360.0f) {
        object_properties.rotation -= 360.0f;
    } else if (object_properties.rotation < 0.0f) {
        object_properties.rotation += 360.0f;
    };

    mutex_object_properties.unlock();

    updateWorldTransformation();
};

void redhand::game_object::setName(std::string name) {
    std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);

    object_properties.name = name;
}

std::string_view redhand::game_object::getName() {
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.name;
}

std::unique_ptr<redhand::game_object> redhand::createCircle(
    glm::vec2 midpoint,
    float radius,
    unsigned int edges,
    glm::vec3 innerColor,
    glm::vec3 outerColor,
    std::shared_ptr<redhand::shader> shade,
    std::shared_ptr<redhand::texture2D> tex,
    std::string name,
    float texture_scale) {

    game_object_properties settings;

    settings.points_coordinates = {{0.5f, 0.5f}};
    settings.point_colors = {innerColor};
    settings.attached_shader = shade;
    settings.attached_texture = tex;
    settings.scale = {radius, radius};
    settings.texture_scale = {texture_scale, texture_scale};
    settings.name = name;

    if (edges <= 2) {
        edges = 120;
    }

    //Set default midpoint
    settings.postition = midpoint - 0.5f * radius;

    settings.triangle_indices.clear();

    settings.points_coordinates.resize(edges + 1);
    settings.point_colors.reserve(edges + 1);
    settings.texture_coordinates.resize(edges + 1);
    settings.triangle_indices.reserve(edges);

    auto fut1 = std::async(std::launch::async, [&]() {
        for (unsigned int i = 1; i <= edges; i++) {
            float dx, dy;
            dx = cosDeg(i * 360 / edges) / 2.0f + 0.5f;
            dy = sinDeg(i * 360 / edges) / 2.0f + 0.5f;

            settings.points_coordinates.at(i) = {dx, dy};
            settings.point_colors.push_back(outerColor);
            settings.texture_coordinates.at(i) = {dx, dy};
        }
    });

    auto fut2 = std::async(std::launch::async, [&]() {
        for (unsigned int i = 0; i < edges - 1; i++) {
            settings.triangle_indices.push_back({0, i + 1, i + 2});
        }
        settings.triangle_indices.push_back({0, edges - 1, 1});
    });

    fut1.wait();
    fut2.wait();

    return std::unique_ptr<redhand::game_object>(new game_object(settings));
}

std::unique_ptr<redhand::game_object> redhand::createRectangle(
    glm::vec2 bottomleft,
    float width,
    float height,
    glm::vec3 color,
    std::shared_ptr<redhand::shader> shade,
    std::shared_ptr<redhand::texture2D> tex,
    redhand::drawing_modes DrawingMode,
    std::string name,
    float texture_scale) {

    game_object_properties settings;

    if (texture_scale <= 0.0f) {
        settings.texture_scale = {-texture_scale, -texture_scale};
        settings.automatic_scaling = true;
    } else {
        settings.texture_scale = {texture_scale, texture_scale};
    }

    settings.attached_shader = shade;
    settings.attached_texture = tex;
    settings.gl_drawing_mode = DrawingMode;
    settings.scale = {width,height};
    settings.name = name;

    settings.points_coordinates = {
        {1.0f, 0.0f}, //top left
        {1.0f, 1.0f}, //top right
        {0.0f, 1.0f}, //bottom right
        {0.0f, 0.0f}  //bottom left
    };

    settings.triangle_indices = {
        {0, 1, 2}, // first triangle
        {0, 2, 3}  // second triangle
    };

    settings.point_colors.resize(4);
    for (int i = 0; i < 4; i++) {
        settings.point_colors.at(i) = color;
    }

    settings.postition = bottomleft;

    return std::unique_ptr<redhand::game_object>(new game_object(settings));
}
