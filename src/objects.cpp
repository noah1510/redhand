#include "objects.hpp"

//minimal constructor
object::object(
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

        shader_routine = [](shader*){};
        LoopFunction = [](GLFWwindow*, object*){};
            
        int point_size = (int)(1.5f * points.size());
        int colors_size = colors.size();
        int texels_size = points.size();

        indices_size = indices.size();

        std::vector <float> data;

        for(int i = 0; i < points.size();i++){
            data.emplace_back(points.at(i));
            if(i % 2 == 1 && i != 0){
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

//minimal with shader routine
object::object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    shader* attached_shader,
    int gl_drawing_mode,

    std::function<void(shader*)> routine
):object(points,indices,colors,attached_shader,gl_drawing_mode){
    shader_routine = routine;
}

//minimal with scale,rotation and position
object::object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    shader* attached_shader,
    int gl_drawing_mode,

    std::vector<float> scaler,
    float rotator,
    std::vector<float> postitions
):object(points, indices, colors, attached_shader, gl_drawing_mode){
    object_scale = scaler;
    object_rotation = rotator;
    if (postitions.size() != 2){
        object_position = {0.0f, 0.0f};
    }else{
        object_position = postitions;
    }
    
}

//minimal with scale,rotation, position and shader routine
object::object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    shader* attached_shader,
    int gl_drawing_mode,

    std::function<void(shader*)> routine,

    std::vector<float> scaler,
    float rotator,
    std::vector<float> postitions
):object(points, indices, colors, attached_shader, gl_drawing_mode,routine){
    object_scale = scaler;
    object_rotation = rotator;
    if (postitions.size() != 2){
        object_position = {0.0f, 0.0f};
    }
    object_position = postitions;
}

//full constructor without texels
object::object(
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
):object(points, indices, colors, attached_shader, gl_drawing_mode,routine,scaler,rotator,postitions){
    if (texture != NULL && texture != nullptr){
        object_textures.emplace_back(texture);
    }
}

//full constructor
object::object(
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
    if(object_shader == nullptr || object_shader == NULL || texels.size() != points.size()){
        errored = true;
    }else{

        shader_routine = [](shader*){};
        LoopFunction = [](GLFWwindow*, object*){};
            
        int point_size = points.size();
        int colors_size = colors.size();
        int texels_size = points.size();
        if(texels.size() != 0){
            texels_size = texels.size();
        }
        
        indices_size = indices.size();

        std::vector <float> data;
        for(int i = 0; i < points.size();i++){
            data.emplace_back(points.at(i));
            if(i % 2 == 1 && i != 0){
                data.emplace_back(points.at(0.0f));
            }
        }
        data.insert(data.end(), colors.begin(), colors.end());
        if(texels.size() != 0){
            data.insert(data.end(), texels.begin(), texels.end());
        }else{
            data.insert(data.end(), points.begin(), points.end());
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

    }
}


object::~object(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void object::draw(){
    //enable texture shader
    object_shader->use();

    object_shader->setInt("useTexture", 2);
    //bind texture and draw background
    if(object_textures.size() != 0){
        for(int i = 0;i < object_textures.size();i++){
            object_textures[i]->bind(i);
        }
    }else{
        object_shader->setInt("useTexture", 0);
    }

    //Create World transformation matrix
    
    glm::mat4 worldTrans = glm::mat4(1.0f);
    worldTrans = glm::translate(worldTrans, glm::vec3(object_position[0],object_position[1],0.0f));
    worldTrans = glm::rotate(worldTrans, glm::radians(object_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    worldTrans = glm::scale(worldTrans, glm::vec3(object_scale.at(0), object_scale.at(1), 1.0f));

    unsigned int transformLoc = glGetUniformLocation(object_shader->getID(), "worldTransformation");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(worldTrans));


    //run the custom shader routine
    shader_routine(object_shader);

    //actually draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);

}

bool object::hasErrord(){
    return errored;
}

void object::onLoop(GLFWwindow* window){
    object_shader->use();
    LoopFunction(window, this);
};

void object::setShaderRoutine(std::function<void(shader*)> routine){
    shader_routine = routine;
};
void object::setLoopFunction(std::function<void(GLFWwindow* window, object* obj)> loop){
    LoopFunction = loop;
};

shader* object::getShader(){
    return object_shader;
};

std::vector<float> object::getPosition(){
    return object_position;
};

void object::setPosition(std::vector<float> pos){
    object_position = pos;
};

void object::move(std::vector<float> delta_pos){
    if(delta_pos.size() == 2){
        object_position.at(0) += delta_pos.at(0);
        object_position.at(1) += delta_pos.at(1);
    }
};

float object::getRotation(){
    return object_rotation;
};
void object::setRotation(float rot){
    object_rotation = rot;

    while(object_rotation > 360.0f){
        object_rotation -= 360.0f;
    }
    while(object_rotation < 0.0f){
        object_rotation += 360.0f;
    }
};
void object::rotate(float delta_rot){
    object_rotation += delta_rot;

    while(object_rotation > 360.0f){
        object_rotation -= 360.0f;
    }
    while(object_rotation < 0.0f){
        object_rotation += 360.0f;
    }
};

object* createHouse(
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
        //color changing
        float timeValue = glfwGetTime();
        float redValue = (cos(timeValue) / 2.0f) + 0.5f;
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        float blueValue = (sin(timeValue + 3.1415f) / 2.0f) + 0.5f;

        shade->setFloat("ourColor", redValue, greenValue, blueValue, 1.0f);
    };

    return new object(points,indices,colors,shade,GL_DYNAMIC_DRAW,routine,{0.5f,0.5f},0.0f,{0.0f,0.0f},texture,texels);
    
}

object* createCircle( 
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
    std::vector<float> position = {0.1f, 0.1f};
    if(midpoint != NULL){
        position.at(0) = midpoint[0]-0.5f;
        position.at(1) = midpoint[1]-0.5f;
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

    return new object(points, indices, colors, shade, GL_STATIC_DRAW, [](shader*){}, {radius,radius}, 0.0f, position, tex, texels);
}

object* createRecktangle(
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
        textureScale * 1.0f,  textureScale * 0.0f,        //top left
        textureScale * 1.0f,  textureScale * 1.0f, //top right
        textureScale * 0.0f,  textureScale * 1.0f, //bottom right
        textureScale * 0.0f,  textureScale * 0.0f         //bottom left
    };

    std::vector<float> position_vector = {bottomleft[0],bottomleft[1]};

    return new object(points,indices,colors,shade,DrawingMode,[](shader*){},{width,height},0.0f,position_vector,tex,texels);
}

float degToRad(float val){
    return val/180*3.1415;
}

float sinDeg(float deg){
    return sin(degToRad(deg));
}

float cosDeg(float deg){
    return cos(degToRad(deg));
}