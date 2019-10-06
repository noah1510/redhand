#include "objects.hpp"

object::object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    std::vector <float> texels,
    shader* attached_shader,
    texture2D* texture,
    int gl_drawing_mode
){

    shade = attached_shader;
    if(shade == nullptr || shade == NULL){
        errored = true;
    }else{

        if(texture != nullptr && texture != NULL){textures.insert(textures.begin(), texture); };
        shader_routine = [](shader*){};
        LoopFunction = [](GLFWwindow*, object*){};
            
        int point_size = points.size();
        int colors_size = colors.size();
        int texels_size = texels.size();

        indices_size = indices.size();

        std::vector <float> data;
        data.insert(data.begin(), points.begin(), points.end());
        data.insert(data.end(), colors.begin(), colors.end());
        data.insert(data.end(), texels.begin(), texels.end());

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

object::object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    std::vector <float> texels,
    shader* attached_shader,
    texture2D* texture,
    int gl_drawing_mode,
    std::function<void(shader*)> routine
):object(points,indices,colors,texels,attached_shader,texture,gl_drawing_mode){
    shader_routine = routine;
}

object::object(
    std::vector <float> points, 
    std::vector <unsigned int> indices,
    std::vector <float> colors,
    std::vector <float> texels,
    shader* attached_shader,
    texture2D* texture,
    int gl_drawing_mode,

    std::function<void(shader*)> routine,

    float scaler,
    float rotator,
    std::vector<float> postitions
):object(points, indices, colors, texels, attached_shader, texture, gl_drawing_mode, routine){
    scale = scaler;
    rotation = rotator;
    if (postitions.size() != 2){
        errored = true;
    }
    position = postitions;

    objectVersion = 1;
}

object::~object(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void object::draw(){
    //enable texture shader
    shade->use();

    shade->setInt("useTexture", 2);
    //bind texture and draw background
    if(textures.size() != 0){
        for(int i = 0;i < textures.size();i++){
            textures[i]->bind(i);
        }
    }else{
        shade->setInt("useTexture", 0);
    }

    //Create World transformation matrix
    if (objectVersion > 0){
        glm::mat4 worldTrans = glm::mat4(1.0f);
        worldTrans = glm::translate(worldTrans, glm::vec3(position[0],position[1],0.0f));
        worldTrans = glm::rotate(worldTrans, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        worldTrans = glm::scale(worldTrans, glm::vec3(scale, scale, 1.0f));

        unsigned int transformLoc = glGetUniformLocation(shade->ID, "worldTransformation");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(worldTrans));
    }

    //run the custom shader routine
    shader_routine(shade);

    //actually draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);

}

bool object::hasErrord(){
    return errored;
}

void object::onLoop(GLFWwindow* window){
    shade->use();
    LoopFunction(window, this);
};

void object::setShaderRoutine(std::function<void(shader*)> routine){
    shader_routine = routine;
};
void object::setLoopFunction(std::function<void(GLFWwindow* window, object* obj)> loop){
    LoopFunction = loop;
};

shader* object::getShader(){
    return shade;
};

std::vector<float> object::getPosition(){
    return position;
};

void object::setPosition(std::vector<float> pos){
    position = pos;
};

float object::getRotation(){
    return rotation;
};
void object::setRotation(float rot){
    rotation = rot;
};

object* createHouse(
    texture2D* texture,
    shader* shade
){
    //Vertex Data
    std::vector <float> points = {
        1.0f,  0.2f, 0.0f,  // top right
        1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  0.2f, 0.0f,   // top left 
        0.0f, 1.0f, 0.0f // top middle
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

    std::vector <float> texels = {
        8.0f, 8.0f,   // top right
        8.0f, 0.0f,  // bottom right
        2.0f, 0.0f,  // bottom left
        2.0f, 8.0f,   // top left 
        5.0f, 10.0f // top middle
    };

    std::function<void(shader*)> routine = [](shader* shade){
        //color changing
        float timeValue = glfwGetTime();
        float redValue = (cos(timeValue) / 2.0f) + 0.5f;
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        float blueValue = (sin(timeValue + 3.1415f) / 2.0f) + 0.5f;

        shade->setFloat("ourColor", redValue, greenValue, blueValue, 1.0f);
    };

    return new object(points,indices,colors,texels,shade,texture,GL_DYNAMIC_DRAW,routine,0.5f,0.0f,{0.0f,0.0f});
    
}

object* createCircle( 
    float midpoint[2],
    float radius,
    int edges,
    float innerColor[3],
    float outerColor[3],
    shader* shade,
    texture2D* tex
){

    std::vector <float> points = {0.0f, 0.0f, 0.0f};
    std::vector <unsigned int> indices;
    std::vector <float> colors = {1.0f, 1.0f, 0.0f};
    std::vector <float> texels = {0.5f, 0.5f};

    if(edges <= 2){
        edges = 120;
    }

    //Set default midpoint
    std::vector<float> position = {0.6f, 0.6f};
    if(midpoint != NULL){
        position.at(0) = midpoint[0];
        position.at(1) = midpoint[1];
    }

    //Set to yellow if NULL
    if(innerColor != NULL){
        for(int i = 0;i < 3;i++){
            colors[i] = innerColor[i];
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
        dx = cosDeg(i*360/edges);
        dy = sinDeg(i*360/edges);

        points.insert(points.end(), dx);
        points.insert(points.end(), dy);
        points.insert(points.end(), 0.0f);

        colors.insert(colors.end(), oColor[0]);
        colors.insert(colors.end(), oColor[1]);
        colors.insert(colors.end(), oColor[2]);

        texels.insert(texels.end(), dx/2 + 0.5f);
        texels.insert(texels.end(), dy/2 + 0.5f);

    }

    for(int i = 0;i < edges;i++){
        indices.insert(indices.end(), 0);
        indices.insert(indices.end(), i + 1);
        indices.insert(indices.end(), i + 2);
        if(indices[i*3 + 2] == edges + 1){indices[i*3 + 2] = 1;};
    }  

    return new object(points, indices, colors, texels, shade, tex, GL_STATIC_DRAW, [](shader*){}, radius, 0.0f, position);
}

object* createRecktangle(
    float topleft[2],
    float width,
    float height,
    float color[3],
    float textureScale,
    shader* shade,
    texture2D* tex,
    int DrawingMode
){
    std::vector<float> points = {
        topleft[0],         topleft[1],          0.0f, //top left
        topleft[0] + width, topleft[1],          0.0f, //top right
        topleft[0] + width, topleft[1] - height, 0.0f, //bottom right
        topleft[0]        , topleft[1] - height, 0.0f, //bottom left
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
        ((topleft[0] / 2.0f) + 0.5f) * textureScale,             (topleft[1]) * textureScale,                             //top left
        (((topleft[0] + width) / 2.0f) + 0.5f) * textureScale,   ((topleft[1] / 2.0f) + 0.5f) * textureScale,             //top right
        (((topleft[0] + width) / 2.0f) + 0.5f) * textureScale,   (((topleft[1] - height) / 2.0f) + 0.5f) * textureScale,  //bottom right
        ((topleft[0] / 2.0f) + 0.5f) * textureScale,             (((topleft[1] - height) / 2.0f) + 0.5f) * textureScale,  //bottom left
    };

    return new object(points,indices,colors,texels,shade,tex,DrawingMode,[](shader*){},1.0f,0.0f,{0.0f,0.0f});
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