#include "redhand/game_object.hpp"

using namespace redhand;

//full constructor
redhand::game_object::game_object(game_object_properties properties ){
    object_properties = properties;

    auto fut1 = std::async(std::launch::async,[&](){
        auto lock1 = std::shared_lock(mutex_object_properties);

        if(object_properties.attached_shader == nullptr || object_properties.attached_shader == NULL ){
            std::cerr << "ERROR::REDHAND::GAME_OBJECT::NO_SHADER" << std::endl;
            triggerError();
        }

        if(object_properties.triangle_indices.size() == 0){
            std::cerr << "ERROR::REDHAND::GAME_OBJECT::NO_TRIANGLES" << std::endl;
            triggerError();
        }
    });
        
    auto less = std::async(std::launch::async,[&](){
        auto lock2 = std::shared_lock(mutex_object_properties);
        for(unsigned int i = 0; i < object_properties.points_coordinates.size(); i++){
            if(object_properties.points_coordinates.at(i).at(0) < 0.0f){
                triggerError();
                std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
            }
            if(object_properties.points_coordinates.at(i).at(1) < 0.0f){
                triggerError();
                std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
            }

            if(hasErrord()){
                break;
            }
        }
    });

    auto more = std::async(std::launch::async,[&](){
        auto lock3 = std::shared_lock(mutex_object_properties);
        for(unsigned int i = 0; i < object_properties.points_coordinates.size(); i++){
            if(object_properties.points_coordinates.at(i).at(0) > 1.0f){
                triggerError();
                std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
            }
            if(object_properties.points_coordinates.at(i).at(1) > 1.0f){
                triggerError();
                std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
            }

            if(hasErrord()){
                break;
            }
        }
    });

    less.wait();
    more.wait();
    fut1.wait();

    if(!has_errored){
        loop_function = [](GLFWwindow*, game_object*){};

        data.reserve(
                object_properties.points_coordinates.size()*3 //size of points
            +   object_properties.points_coordinates.size()*3 //size of colors
            +   object_properties.points_coordinates.size()*2 //size of texels
        );   

        for(unsigned int i = 0; i < object_properties.points_coordinates.size();i++){
            data.push_back(object_properties.points_coordinates.at(i).at(0));
            data.push_back(object_properties.points_coordinates.at(i).at(1));
            data.push_back(0.0f);
        }

        if(object_properties.point_colors.size() != object_properties.points_coordinates.size()){
            for(unsigned int i = 0; i < object_properties.points_coordinates.size();i++){
                data.push_back(0.0f);
                data.push_back(0.0f);
                data.push_back(0.0f);
            }
        }else{
            for(unsigned int i = 0;i < object_properties.points_coordinates.size();i++){
                data.push_back(object_properties.point_colors.at(i).at(0));
                data.push_back(object_properties.point_colors.at(i).at(1));
                data.push_back(object_properties.point_colors.at(i).at(2));
            }
        }

        if(object_properties.texture_coordinates.size() == object_properties.points_coordinates.size()){
            for(unsigned int i = 0;i < object_properties.points_coordinates.size();i++){
                data.push_back(object_properties.texture_coordinates.at(i).at(0));
                data.push_back(object_properties.texture_coordinates.at(i).at(1));
            };
        }else{
            for(unsigned int i = 0;i < object_properties.points_coordinates.size();i++){
                data.push_back(object_properties.points_coordinates.at(i).at(0));
                data.push_back(object_properties.points_coordinates.at(i).at(1));
            }
        }      

        std::vector<unsigned int> trigs;
        trigs.reserve(object_properties.triangle_indices.size()*3);
        for(unsigned int i = 0; i < object_properties.triangle_indices.size();i++){
            trigs.push_back(object_properties.triangle_indices.at(i).at(0));
            trigs.push_back(object_properties.triangle_indices.at(i).at(1));
            trigs.push_back(object_properties.triangle_indices.at(i).at(2));
        }

        //Initilize the buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        ///Create arrays and buffers
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), data.data(), object_properties.gl_drawing_mode);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), data.data(), object_properties.gl_drawing_mode);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*trigs.size(), trigs.data(), object_properties.gl_drawing_mode);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(object_properties.points_coordinates.size() * 3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)((object_properties.points_coordinates.size() * 6) * sizeof(float)));
        glEnableVertexAttribArray(2);

        if (    object_properties.attached_texture != NULL 
            &&  object_properties.attached_texture != nullptr 
            &&  !object_properties.attached_texture->hasErrord()
        ){ texture_mode = 1; };

        updateWorldTransformation();

    }
}


redhand::game_object::~game_object(){
    auto lock = std::scoped_lock(
        mutex_game_object,
        mutex_has_errored,
        mutex_loop_function,
        mutex_object_properties,
        mutex_texture_scale,
        mutex_world_transformation
    );

    try{
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    
}

void redhand::game_object::setScreenSize(int width, int height){
    auto lock1 = std::shared_lock(mutex_game_object);
    if(object_properties.attached_texture == nullptr || object_properties.attached_texture == NULL){
        return;
    }

    auto lock2 = std::shared_lock(mutex_texture_mode);
    if(texture_mode == 1){
        auto lock3 = std::scoped_lock(mutex_texture_scale);
        texture_scale.x = object_properties.texture_scale.x * object_properties.scale.at(0) * width / (object_properties.attached_texture->getWidth() * (width / height + 1.0f));
        texture_scale.y = object_properties.texture_scale.y * object_properties.scale.at(1) * height / (object_properties.attached_texture->getHeight() * (height / width + 1.0f));
    }  
}

void redhand::game_object::draw(){
    auto lock1 = std::shared_lock(mutex_object_properties);
    if(object_properties.attached_shader == nullptr){
        std::cerr << "ERROR::REDHAND::GAME_OBJECT::NULLPTR_AS_SHADER" << std::endl;
    }

    //if there are textures set the texture scale of the shader
    auto lock2 = std::shared_lock(mutex_texture_mode);
    if(texture_mode == 1){
        object_properties.attached_shader->setTextureScale(texture_scale);
    }

    //enable texture shader
    object_properties.attached_shader->use();

    //set the uniform variables
    object_properties.attached_shader->setInt("textureMode", texture_mode);
    object_properties.attached_shader->setFloat("colorAlpha", object_properties.alpha_value);

    //bind texture and draw background
    if(texture_mode == 1){   
        object_properties.attached_texture->bind(0);
    }

    //set World transformation matrix  
    auto lock3 = std::shared_lock(mutex_world_transformation);
    unsigned int transformLoc = glGetUniformLocation(object_properties.attached_shader->getID(), "worldTransformation");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(world_transformation));

    //actually draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, object_properties.triangle_indices.size()*3, GL_UNSIGNED_INT, 0);

    //reset the uniform values just in case
    object_properties.attached_shader->setInt("textureMode", 0);
    object_properties.attached_shader->setFloat("colorAlpha", 1.0f);


}

void redhand::game_object::updateWorldTransformation(){
    auto lock = std::scoped_lock(mutex_world_transformation);
    auto lock2 = std::shared_lock(mutex_object_properties);

    world_transformation = glm::mat4(1.0f);
    world_transformation = glm::translate(world_transformation, glm::vec3(object_properties.postition.at(0),object_properties.postition.at(1),0.0f));
    world_transformation = glm::rotate(world_transformation, glm::radians(object_properties.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    world_transformation = glm::scale(world_transformation, glm::vec3(object_properties.scale.at(0), object_properties.scale.at(1), 1.0f));
}

bool redhand::game_object::hasErrord(){
    std::shared_lock<std::shared_mutex> lock(mutex_has_errored);

    return has_errored;
}

void redhand::game_object::triggerError(){
    std::scoped_lock<std::shared_mutex> lock(mutex_has_errored);

    has_errored = true;
}

void redhand::game_object::setColorAlpha(float alpha){
    if(alpha >= 0.0f && alpha <= 1.0f){
        std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);
        object_properties.alpha_value = alpha;
    }
}

void redhand::game_object::onLoop(GLFWwindow* window){
    std::shared_lock<std::shared_mutex> lock(mutex_loop_function);

    loop_function(window, this);
};

void redhand::game_object::setLoopFunction(std::function<void(GLFWwindow* window, game_object* obj)> loop){
    std::scoped_lock<std::shared_mutex> lock(mutex_loop_function);

    loop_function = loop;
};

std::shared_ptr<redhand::shader> redhand::game_object::getShader(){
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return std::shared_ptr<redhand::shader>(object_properties.attached_shader);
};

std::array<float,2> redhand::game_object::getPosition(){
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.postition;
};

void redhand::game_object::setPosition(std::array<float,2> pos){
    std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);

    object_properties.postition = pos;

    updateWorldTransformation();
};

void redhand::game_object::move(std::array<float,2> delta_pos){
    std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);

    object_properties.postition.at(0) += delta_pos.at(0);
    object_properties.postition.at(1) += delta_pos.at(1);

    updateWorldTransformation();
};

float redhand::game_object::getRotation(){
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.rotation;
};
std::array<float,2> redhand::game_object::getScale(){
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.scale;
};
void redhand::game_object::setRotation(float rot){
    while(rot >= 360.0f){
        rot -= 360.0f;
    }
    while(rot < 0.0f){
        rot += 360.0f;
    }

    std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);

    object_properties.rotation = rot;

    updateWorldTransformation();

};
void redhand::game_object::rotate(float delta_rot){

    while(delta_rot >= 360.0f){
        delta_rot -= 360.0f;
    }
    while(delta_rot < 0.0f){
        delta_rot += 360.0f;
    }

    std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);

    object_properties.rotation += delta_rot;

    if(object_properties.rotation >= 360.0f){
        object_properties.rotation -= 360.0f;
    }else if(object_properties.rotation < 0.0f){
        object_properties.rotation += 360.0f;
    };

    updateWorldTransformation();

};

void redhand::game_object::setName(std::string name){
    std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);

    object_properties.name = name;
}
std::string_view redhand::game_object::getName(){
    std::shared_lock<std::shared_mutex> lock(mutex_object_properties);

    return object_properties.name;
}

std::unique_ptr<redhand::game_object> redhand::createCircle( 
    std::array<float,2> midpoint,
    float radius,
    unsigned int edges,
    std::array<float,3> innerColor,
    std::array<float,3> outerColor,
    std::shared_ptr<redhand::shader> shade,
    std::shared_ptr<redhand::texture2D> tex,
    std::string name,
    float texture_scale
){

    auto settings = DEFAULT_GAME_OBJECT_PROPERTIES;

    settings.points_coordinates = {{0.5f, 0.5f}};
    settings.point_colors = {innerColor};
    settings.attached_shader = shade;
    settings.attached_texture = tex;
    settings.scale = {radius,radius};
    settings.texture_scale = {texture_scale,texture_scale};
    settings.name = name;

    if(edges <= 2){
        edges = 120;
    }

    //Set default midpoint
    settings.postition = {{0.0f, 0.0f}};
    if(midpoint.size() == 2){
        settings.postition.at(0) = midpoint[0]-0.5f*radius;
        settings.postition.at(1) = midpoint[1]-0.5f*radius;
    }

    settings.triangle_indices.clear();

    settings.points_coordinates.resize(edges+1);
    settings.point_colors.reserve(edges+1);
    settings.texture_coordinates.resize(edges+1);
    settings.triangle_indices.reserve(edges);

    for(unsigned int i = 1; i <= edges;i++){
        float dx,dy;
        dx = cosDeg(i*360/edges)/2.0f + 0.5f;
        dy = sinDeg(i*360/edges)/2.0f + 0.5f;

        settings.points_coordinates.at(i) = {dx,dy};
        settings.point_colors.push_back(outerColor);
        settings.texture_coordinates.at(i) = {dx,dy};
    }

    for(unsigned int i = 0;i < edges-1;i++){
        settings.triangle_indices.push_back({0,i+1,i+2});
        }   
    
    settings.triangle_indices.push_back({0,edges-1,1});

    auto obj = std::unique_ptr<redhand::game_object>(new game_object(settings));

    ///@todo set the texture scale

    return std::move(obj);
}

std::unique_ptr<redhand::game_object> redhand::createRectangle(
    std::array<float,2> bottomleft,
    float width,
    float height,
    std::array<float,3> color,
    std::shared_ptr<redhand::shader> shade,
    std::shared_ptr<redhand::texture2D> tex,
    int DrawingMode,
    std::string name,
    float texture_scale
){

    auto settings = DEFAULT_GAME_OBJECT_PROPERTIES;
    settings.attached_shader = shade;
    settings.attached_texture = tex;
    settings.gl_drawing_mode = DrawingMode;
    settings.scale.at(0) = width;
    settings.scale.at(1) = height;
    settings.texture_scale = {texture_scale,texture_scale};
    settings.name = name;

    settings.points_coordinates = {
        {1.0f,  0.0f}, //top left
        {1.0f,  1.0f}, //top right
        {0.0f,  1.0f}, //bottom right
        {0.0f,  0.0f}  //bottom left
    };

    settings.triangle_indices = {
        {0, 1, 2},   // first triangle
        {0, 2, 3}    // second triangle
    };

    settings.point_colors.resize(4);
    for(int i = 0;i < 4;i++){
        settings.point_colors.at(i) = color;
    }

    settings.postition = bottomleft;

    auto obj = std::unique_ptr<redhand::game_object>(new game_object(settings));

    ///@todo set the texture scale
    
    return std::move(obj);
}
