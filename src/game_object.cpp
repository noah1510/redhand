#include "game_object.hpp"

//minimal constructor
game_object::game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode
){
    object_shader = attached_shader;
    if(object_shader == nullptr || object_shader == NULL){
        errored = true;
    }else{
        for(int i = 0; i < points.size(); i++){
            if(points.at(i) < 0.0f || points.at(i) > 1.0f){
                errored = true;
                break;
            }
        }
    }

    if(!errored){

        shader_routine = [](shader*){};
        LoopFunction = [](GLFWwindow*, game_object*){};
            
        int point_size = (int)(1.5f * points.size());
        int colors_size = colors.size();
        int texels_size = points.size();

        indices_size = indices.size();

        std::vector <float> data;

        for(int i = 0; i < points.size();i++){
            data.emplace_back(points.at(i));
            if(i % 2 == 1){
                data.emplace_back(points.at(0.0f));
            }
        }
        data.insert(data.end(), colors.begin(), colors.end());
        data.insert(data.end(), points.begin(), points.end());

        //Initilize the buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        ///Create arrays and buffers
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), data.data(), gl_drawing_mode);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), data.data(), gl_drawing_mode);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), indices.data(), gl_drawing_mode);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(point_size * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)((point_size + colors_size) * sizeof(float)));
        glEnableVertexAttribArray(2);

    }
}

//minimal with name
game_object::game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,
        std::string name):game_object(points, indices, colors, attached_shader, gl_drawing_mode){
            object_name = name;
        }

//minimal with shader routine
game_object::game_object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    shader* attached_shader,
    int gl_drawing_mode,

    std::function<void(shader*)> routine
):game_object(points,indices,colors,attached_shader,gl_drawing_mode){
    shader_routine = routine;
}

//minimal with scale,rotation and position
game_object::game_object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    shader* attached_shader,
    int gl_drawing_mode,

    std::vector<float> scaler,
    float rotator,
    std::vector<float> postitions
):game_object(points, indices, colors, attached_shader, gl_drawing_mode){
    object_scale = scaler;
    object_rotation = rotator;
    if (postitions.size() != 2){
        object_position = {0.0f, 0.0f};
    }else{
        object_position = postitions;
    }
    
}

//minimal with scale,rotation, position and shader routine
game_object::game_object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    shader* attached_shader,
    int gl_drawing_mode,

    std::function<void(shader*)> routine,

    std::vector<float> scaler,
    float rotator,
    std::vector<float> postitions
):game_object(points, indices, colors, attached_shader, gl_drawing_mode,routine){
    object_scale = scaler;
    object_rotation = rotator;
    if (postitions.size() != 2){
        object_position = {0.0f, 0.0f};
    }
    object_position = postitions;
}

//full constructor without texels
game_object::game_object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    shader* attached_shader,
    int gl_drawing_mode,

    std::function<void(shader*)> routine,

    std::vector<float> scaler,
    float rotator,
    std::vector<float> postitions,

    texture2D* texture
):game_object(points, indices, colors, attached_shader, gl_drawing_mode,routine,scaler,rotator,postitions){
    if (texture != NULL && texture != nullptr && !texture->hasErrord()){
        textureMode = 1;
        object_texture = texture;
    }
}

//full constructor
game_object::game_object(
        std::vector <float> points, 
        std::vector <unsigned int> indices,
        std::vector <float> colors,
        shader* attached_shader,
        int gl_drawing_mode,

        std::function<void(shader*)> routine,

        std::vector<float> scaler,
        float rotator,
        std::vector<float> postitions,

        texture2D* texture,
        std::vector <float> texels
){
    object_shader = attached_shader;

    if(object_shader == nullptr || object_shader == NULL ){
        errored = true;
    }else{
        for(int i = 0; i < points.size(); i++){
            if(points.at(i) < 0.0f || points.at(i) > 1.0f){
                errored = true;
                break;
            }
        }
    }

    if(!errored){
        shader_routine = [](shader*){};
        LoopFunction = [](GLFWwindow*, game_object*){};
            
        int point_size = (int)(1.5f * points.size());
        int colors_size = colors.size();
        int texels_size = points.size();
        if(texels.size() != 0 && texels.size() == points.size()){
            texels_size = texels.size();
        }
        
        indices_size = indices.size();

        std::vector <float> data;
        for(int i = 0; i < points.size();i++){
            data.emplace_back(points.at(i));
            if(i % 2 == 1){
                data.emplace_back(points.at(0.0f));
            }
        }

        data.insert(data.end(),colors.begin(),colors.end());

        if(texels.size() != 0 && texels.size() == points.size()){
            data.insert(data.end(),texels.begin(),texels.end());
        }else{
            data.insert(data.end(),points.begin(),points.end());
        }
        

        //Initilize the buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        ///Create arrays and buffers
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), data.data(), gl_drawing_mode);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), data.data(), gl_drawing_mode);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), indices.data(), gl_drawing_mode);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(point_size * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)((point_size + colors_size) * sizeof(float)));
        glEnableVertexAttribArray(2);

        shader_routine = routine;

        object_rotation = rotator;
        object_scale = scaler;
        if (postitions.size() != 2){
            object_position = {0.0f, 0.0f};
        }else{
            object_position = postitions;
        }

        if (texture != NULL && texture != nullptr && !texture->hasErrord()){
            textureMode = 1;
            object_texture = texture;
        }

    }
}


game_object::~game_object(){
    auto lock = std::scoped_lock(gameObjectLock,nameLock,positionLock,rotationLock,textureScaleLock,scaleLock);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void game_object::setScreenSize(int width, int height){
    if(textureMode == 1 && object_texture != nullptr && object_texture != NULL){
        texture_scale.x = object_scale.at(0) * width / (object_texture->getWidth() * (width / height + 1.0f));
        texture_scale.y = object_scale.at(1) * height / (object_texture->getHeight() * (height / width + 1.0f));
    }  
}

void game_object::draw(){
    auto lock = std::shared_lock(gameObjectLock);

    //if there are textures set the texture scale of the shader
    if(textureMode == 1){
        object_shader->setTextureScale(texture_scale);
    }

    //enable texture shader
    object_shader->use();

    //set the uniform variables
    object_shader->setInt("textureMode", textureMode);
    object_shader->setFloat("colorAlpha", colorAlphaValue);

    //bind texture and draw background
    if(textureMode == 1){   
        object_texture->bind(0);
    }

    //Create World transformation matrix  
    glm::mat4 worldTrans = glm::mat4(1.0f);
    worldTrans = glm::translate(worldTrans, glm::vec3(getPosition().at(0),getPosition().at(1),0.0f));
    worldTrans = glm::rotate(worldTrans, glm::radians(getRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
    worldTrans = glm::scale(worldTrans, glm::vec3(getScale().at(0), getScale().at(1), 1.0f));

    unsigned int transformLoc = glGetUniformLocation(object_shader->getID(), "worldTransformation");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(worldTrans));

    //run the custom shader routine
    shader_routine(object_shader);

    //actually draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);

    //reset the uniform values just in case
    object_shader->setInt("textureMode", 0);
    object_shader->setFloat("colorAlpha", 1.0f);


}

bool game_object::hasErrord(){
    std::shared_lock<std::shared_mutex> lock(gameObjectLock);
    return errored;
}

void game_object::setColorAlpha(float alpha){
    std::scoped_lock<std::shared_mutex> lock(gameObjectLock);
    if(alpha >= 0.0f && alpha <= 1.0f){
        colorAlphaValue = alpha;
    }
}

void game_object::onLoop(GLFWwindow* window){
    std::shared_lock<std::shared_mutex> lock(gameObjectLock);
    //object_shader->use();
    LoopFunction(window, this);
};

void game_object::setShaderRoutine(std::function<void(shader*)> routine){
    std::scoped_lock<std::shared_mutex> lock(gameObjectLock);
    shader_routine = routine;
};
void game_object::setLoopFunction(std::function<void(GLFWwindow* window, game_object* obj)> loop){
    std::scoped_lock<std::shared_mutex> lock(gameObjectLock);
    LoopFunction = loop;
};

shader* game_object::getShader(){
    std::shared_lock<std::shared_mutex> lock(gameObjectLock);
    return object_shader;
};

std::vector<float> game_object::getPosition(){
    std::shared_lock<std::shared_mutex> lock(positionLock);
    return object_position;
};

void game_object::setPosition(std::vector<float> pos){
    std::scoped_lock<std::shared_mutex> lock(positionLock);
    object_position = pos;
};

void game_object::move(std::vector<float> delta_pos){
    std::scoped_lock<std::shared_mutex> lock(positionLock);
    if(delta_pos.size() == 2){
        object_position.at(0) += delta_pos.at(0);
        object_position.at(1) += delta_pos.at(1);
    }
};

float game_object::getRotation(){
    std::shared_lock<std::shared_mutex> lock(rotationLock);
    return object_rotation;
};
std::vector<float> game_object::getScale(){
    std::shared_lock<std::shared_mutex> lock(scaleLock);
    return object_scale;
};
void game_object::setRotation(float rot){
    std::scoped_lock<std::shared_mutex> lock(rotationLock);
    object_rotation = rot;

    while(object_rotation > 360.0f){
        object_rotation -= 360.0f;
    }
    while(object_rotation < 0.0f){
        object_rotation += 360.0f;
    }
};
void game_object::rotate(float delta_rot){
    std::scoped_lock<std::shared_mutex> lock(rotationLock);
    object_rotation += delta_rot;

    while(object_rotation > 360.0f){
        object_rotation -= 360.0f;
    }
    while(object_rotation < 0.0f){
        object_rotation += 360.0f;
    }
};

void game_object::setName(std::string name){
    std::scoped_lock<std::shared_mutex> lock(nameLock);
    object_name = name;
}
std::string game_object::getName(){
    std::shared_lock<std::shared_mutex> lock(nameLock);
    return object_name;
}

game_object* createHouse(
    texture2D* texture,
    shader* shade,
    float texture_scale
){
    //Vertex Data
    std::vector <float> points = {
        1.0f, 0.55f,  // top right
        1.0f, 0.0f,  // bottom right
        0.0f, 0.0f,  // bottom left
        0.0f, 0.55f,  // top left 
        0.5f, 1.0f,  // top middle
    };
    std::vector <unsigned int> indices = {
        0, 1, 3,   // first triangle
        1, 2, 3,    // second triangle
        0, 3, 4     //third triangle
    };
    std::vector <float> colors = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f };

    std::vector <float> texels;
    for(int i = 0;i < points.size();i++){
        texels.emplace_back(points.at(i)*texture_scale);
    }

    std::function<void(shader*)> routine = [](shader* shade){
        shade->setInt("useTexture", 1);
    };

    return new game_object(points,indices,colors,shade,GL_DYNAMIC_DRAW,routine,{0.5f,0.5f},0.0f,{0.0f,0.0f},texture,texels);
    
}

game_object* createCircle( 
    float midpoint[2],
    float radius,
    int edges,
    float innerColor[3],
    float outerColor[3],
    shader* shade,
    texture2D* tex,
    float texture_scale
){

    std::vector <float> points = {0.5f, 0.5f};
    std::vector <unsigned int> indices;
    std::vector <float> colors = {1.0f, 1.0f, 0.0f};
    std::vector <float> texels = {0.5f*texture_scale, 0.5f*texture_scale};

    if(edges <= 2){
        edges = 120;
    }

    //Set default midpoint
    std::vector<float> position = {0.0f, 0.0f};
    if(midpoint != NULL){
        position.at(0) = midpoint[0]-0.5f*radius;
        position.at(1) = midpoint[1]-0.5f*radius;
    }

    //Set to yellow if NULL
    if(innerColor != NULL){
        for(int i = 0;i < 3;i++){
            colors.at(i) = innerColor[i];
        }
    }

    //Set to orange if NULL
    float oColor[3] = {1.0f, 0.3f, 0.0f};
    if(outerColor != NULL){
        for(int i = 0;i < 3;i++){
            oColor[i] = outerColor[i];
        }
    }

    for(int i = 0; i < edges;i++){
        float dx,dy;
        dx = cosDeg(i*360/edges)/2.0f + 0.5f;
        dy = sinDeg(i*360/edges)/2.0f + 0.5f;

        points.insert(points.end(), dx);
        points.insert(points.end(), dy);

        colors.insert(colors.end(), oColor[0]);
        colors.insert(colors.end(), oColor[1]);
        colors.insert(colors.end(), oColor[2]);

        texels.insert(texels.end(), dx*texture_scale);
        texels.insert(texels.end(), dy*texture_scale);

    }

    for(int i = 0;i < edges;i++){
        indices.insert(indices.end(), 0);
        indices.insert(indices.end(), i + 1);
        indices.insert(indices.end(), i + 2);
        if(indices.at(i*3 + 2) == edges + 1){indices.at(i*3 + 2) = 1;};
    }  

    return new game_object(points, indices, colors, shade, GL_STATIC_DRAW, [](shader*){}, {radius,radius}, 0.0f, position, tex, texels);
}

game_object* createRecktangle(
    float bottomleft[2],
    float width,
    float height,
    float color[3],
    shader* shade,
    texture2D* tex,
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

    return new game_object(points,indices,colors,shade,DrawingMode,[](shader*){},{width,height},0.0f,position_vector,tex,texels);
}
