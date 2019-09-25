#include "objects.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    shader_routine(shade);

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

object* createHouse(
    texture2D* texture,
    shader* shade
){
    //Vertex Data
    std::vector <float> points = {
        0.4f,  0.10f, 0.0f,  // top right
        0.4f, -0.5f, 0.0f,  // bottom right
        -0.4f, -0.5f, 0.0f,  // bottom left
        -0.4f,  0.10f, 0.0f,   // top left 
        0.0f, 0.45f, 0.0f // top middle
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
        0.8f, 0.8f,   // top right
        0.8f, 0.0f,  // bottom right
        0.2f, 0.0f,  // bottom left
        0.2f, 0.8f,   // top left 
        0.5f, 1.0f // top middle
    };

    std::function<void(shader*)> routine = [](shader* shade){
        //color changing
        float timeValue = glfwGetTime();
        float redValue = (cos(timeValue) / 2.0f) + 0.5f;
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        float blueValue = (sin(timeValue + 3.1415f) / 2.0f) + 0.5f;

        shade->setFloat("ourColor", redValue, greenValue, blueValue, 1.0f);
    };

    return new object(points,indices,colors,texels,shade,texture,GL_DYNAMIC_DRAW,routine);
    
}

object* createCircle( 
    float midpoint[3],
    float radius,
    int edges,
    float innerColor[3],
    float outerColor[3],
    shader* shade,
    texture2D* tex
){

    std::vector <float> points = {0.6f, 0.6f, 0.0f};
    std::vector <unsigned int> indices;
    std::vector <float> colors = {1.0f, 1.0f, 0.0f};
    std::vector <float> texels = {0.5f, 0.5f};

    if(edges <= 2){
        edges = 120;
    }

    //Set default midpoint
    if(midpoint != NULL){
        for(int i = 0;i < 3;i++){
            points[i] = midpoint[i];
        }
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

        points.insert(points.end(), dx * radius + points[0]);
        points.insert(points.end(), dy * radius + points[1]);
        points.insert(points.end(), points[2]);

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

    return new object(points,indices,colors,texels,shade,tex,GL_STATIC_DRAW);
}

object* createColorTextureRectangle(
    float topRightCorner[3],
    float bottomLeftCorner[3],
    float colorTopRight[3],
    float colorBottomLeft[3],
    float texel2DTopRight[2],
    float texel2DBottomLeft[2],
    shader* shade,
    texture2D* tex,
    int DrawingMode
){

    if(topRightCorner == NULL || bottomLeftCorner == NULL){
        return nullptr;
    } 

    if(texel2DBottomLeft == NULL || texel2DTopRight == NULL){
        return nullptr;
    }

    float tColor[3] = {0.2f, 0.3f, 0.3f};
    if(colorTopRight != NULL){
        for(int i = 0;i < 3;i++){
            tColor[i]  = colorTopRight[i];
        }
    }

    float bColor[3] = {0.2f, 0.3f, 0.3f};
    if(colorBottomLeft != NULL){
        for(int i = 0;i < 3;i++){
            bColor[i]  = colorBottomLeft[i];
        }
    }

    std::vector<float> points = {
        topRightCorner[0],   bottomLeftCorner[1], bottomLeftCorner[2], // bottom right
        bottomLeftCorner[0], bottomLeftCorner[1], bottomLeftCorner[2], // bottom left
        topRightCorner[0],   topRightCorner[1],   topRightCorner[2],   // top right
        bottomLeftCorner[0], topRightCorner[1],   topRightCorner[2]   // top left
    };

    std::vector<float> colors = {
        (tColor[0] + bColor[0])/2.0f, (tColor[1] + bColor[1])/2.0f, (tColor[2] + bColor[2])/2.0f, // bottom right
        bColor[0], bColor[1], bColor[2],// bottom left
        tColor[0], tColor[1], tColor[2],// top right
        (tColor[0] + bColor[0])/2.0f, (tColor[1] + bColor[1])/2.0f, (tColor[2] + bColor[2])/2.0f // top left
        
    };

    std::vector<float> texels = {
        texel2DTopRight[0],   texel2DBottomLeft[1], // bottom right
        texel2DBottomLeft[0], texel2DBottomLeft[1], // bottom left
        texel2DTopRight[0],   texel2DTopRight[1],   // top right
        texel2DBottomLeft[0], texel2DTopRight[1]    // top left
    };


    std::vector<unsigned int> indices = {
        0, 1, 2,   // first triangle
        1, 2, 3    // second triangle
    };

    return new object(points,indices,colors,texels,shade,tex,DrawingMode);

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