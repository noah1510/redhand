#include "redhand/game_object.hpp"

using namespace redhand;

//full constructor
redhand::game_object::game_object(game_object_properties properties ){
    object_properties = properties;

    auto fut1 = std::async(std::launch::async,[&](){
        auto lock = std::shared_lock(mutex_object_properties);

        if(object_properties.attached_shader == nullptr || object_properties.attached_shader == NULL ){
            std::cerr << "ERROR::REDHAND::GAME_OBJECT::NO_SHADER" << std::endl;
            triggerError();
        }

        if(object_properties.triangle_indices.size() == 0){
            std::cerr << "ERROR::REDHAND::GAME_OBJECT::NO_TRIANGLES" << std::endl;
            triggerError();
        }
    });

    auto fut2 = std::async(std::launch::async,[&](){
        auto lock = std::shared_lock(mutex_object_properties);
        
        for(unsigned int i = 0; i < object_properties.points_coordinates.size(); i++){
            auto less0 = std::async(std::launch::async,[&](){
                if(object_properties.points_coordinates.at(i).at(0) < 0.0f){
                    triggerError();
                    std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
                }
            });

            auto more0 = std::async(std::launch::async,[&](){
                if(object_properties.points_coordinates.at(i).at(0) > 1.0f){
                    triggerError();
                    std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
                }
            });

            auto less1 = std::async(std::launch::async,[&](){
                if(object_properties.points_coordinates.at(i).at(1) < 0.0f){
                    triggerError();
                    std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
                }
            });

            auto more1 = std::async(std::launch::async,[&](){
                if(object_properties.points_coordinates.at(i).at(1) > 1.0f){
                    triggerError();
                    std::cerr << "ERROR::REDHAND::GAME_OBJECT::INVALID_LOCAL_COORDINATE" << std::endl;
                }
            });

            less0.wait();
            more0.wait();
            less1.wait();
            more1.wait();

            if(hasErrord()){
                break;
            }

        }
    });

    fut1.wait();
    fut2.wait();

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

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*object_properties.triangle_indices.size(), object_properties.triangle_indices.data(), object_properties.gl_drawing_mode);

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

    }
}


redhand::game_object::~game_object(){
    auto lock = std::scoped_lock(mutex_game_object,mutex_has_errored,mutex_loop_function,mutex_object_properties);
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
    if(texture_mode == 1 && object_properties.attached_texture != nullptr && object_properties.attached_texture != NULL){
        object_properties.texture_scale.x = object_properties.scale.at(0) * width / (object_properties.attached_texture->getWidth() * (width / height + 1.0f));
        object_properties.texture_scale.y = object_properties.scale.at(1) * height / (object_properties.attached_texture->getHeight() * (height / width + 1.0f));
    }  
}

void redhand::game_object::draw(){
    auto lock = std::shared_lock(mutex_object_properties);

    //if there are textures set the texture scale of the shader
    if(texture_mode == 1){
        object_properties.attached_shader->setTextureScale(object_properties.attached_texture->getTextureScale());
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

    //Create World transformation matrix  
    glm::mat4 worldTrans = glm::mat4(1.0f);
    worldTrans = glm::translate(worldTrans, glm::vec3(getPosition().at(0),getPosition().at(1),0.0f));
    worldTrans = glm::rotate(worldTrans, glm::radians(getRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
    worldTrans = glm::scale(worldTrans, glm::vec3(getScale().at(0), getScale().at(1), 1.0f));

    unsigned int transformLoc = glGetUniformLocation(object_properties.attached_shader->getID(), "worldTransformation");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(worldTrans));

    //actually draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, object_properties.triangle_indices.size(), GL_UNSIGNED_INT, 0);

    //reset the uniform values just in case
    object_properties.attached_shader->setInt("textureMode", 0);
    object_properties.attached_shader->setFloat("colorAlpha", 1.0f);


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
};

void redhand::game_object::move(std::array<float,2> delta_pos){
    std::scoped_lock<std::shared_mutex> lock(mutex_object_properties);

    object_properties.postition.at(0) += delta_pos.at(0);
    object_properties.postition.at(1) += delta_pos.at(1);
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
    float texture_scale
){

    auto settings = DEFAULT_GAME_OBJECT_PROPERTIES;

    settings.points_coordinates = {{0.5f, 0.5f}};
    settings.point_colors = {{1.0f, 1.0f, 0.0f}};
    settings.texture_coordinates = {{0.5f, 0.5f}};

    if(edges <= 2){
        edges = 120;
    }

    //Set default midpoint
    settings.postition = {{0.0f, 0.0f}};
    if(midpoint.size == 2){
        settings.postition.at(0) = midpoint[0]-0.5f*radius;
        settings.postition.at(1) = midpoint[1]-0.5f*radius;
    }

    //Set to yellow if NULL
    if(innerColor.size() == 2){
        for(int i = 0;i < 3;i++){
            settings.point_colors.at(0).at(i) = innerColor[i];
        }
    }

    //Set to orange if NULL
    std::array<float,3> oColor = {1.0f, 0.3f, 0.0f};
    if(outerColor.size == 3){
        for(int i = 0;i < 3;i++){
            oColor[i] = outerColor[i];
        }
    }

    settings.points_coordinates.reserve(edges);
    settings.point_colors.reserve(edges);
    settings.texture_coordinates.reserve(edges);
    settings.triangle_indices.reserve(edges);

    for(unsigned int i = 0; i < edges;i++){
        float dx,dy;
        dx = cosDeg(i*360/edges)/2.0f + 0.5f;
        dy = sinDeg(i*360/edges)/2.0f + 0.5f;

        settings.points_coordinates.push_back({dx,dy});
        settings.point_colors.push_back(oColor);
        settings.texture_coordinates.push_back({dx*texture_scale,dy*texture_scale})
    }

    for(unsigned int i = 0;i < edges;i++){
        settings.triangle_indices.push_back({0,i+1,i+2});
        if(settings.triangle_indices.at(i).at(2) == edges + 1){settings.triangle_indices.at(i).at(2) = 1;};
    }  

    settings.points_coordinates.shrink_to_fit();
    settings.point_colors.shrink_to_fit();
    settings.texture_coordinates.shrink_to_fit();
    settings.triangle_indices.shrink_to_fit();

    return std::unique_ptr<redhand::game_object>(new game_object(settings));
}

std::unique_ptr<redhand::game_object> redhand::createRecktangle(
    float bottomleft[2],
    float width,
    float height,
    float color[3],
    std::shared_ptr<redhand::shader> shade,
    std::shared_ptr<redhand::texture2D> tex,
    int DrawingMode,
    float textureScale
){

    std::vector<float> points = {
        0.0f + 1.0f,  0.0f,        //top left
        0.0f + 1.0f,  0.0f + 1.0f, //top right
        0.0f,         0.0f + 1.0f, //bottom right
        0.0f,         0.0f         //bottom left
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,   // first triangle
        0, 2, 3    // second triangle
    };

    std::vector<float> colors;

    for(int i = 0;i < 4;i++){
        colors.emplace_back(color[0]);
        colors.emplace_back(color[1]);
        colors.emplace_back(color[2]);
    }


    std::vector<float> texels = {
        1.0f,  0.0f, //top left
        1.0f,  1.0f, //top right
        0.0f,  1.0f, //bottom right
        0.0f,  0.0f  //bottom left
    };

    std::vector<float> position_vector = {bottomleft[0],bottomleft[1]};

    if(tex != nullptr){
        tex->setTextureScale(glm::vec2{textureScale,textureScale});
    }

    auto obj = new game_object(points,indices,colors,shade,DrawingMode,[](std::shared_ptr<redhand::shader>){},{width,height},0.0f,position_vector,tex,texels);

    return obj;
}
