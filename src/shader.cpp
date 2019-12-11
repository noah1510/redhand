#include "shader.hpp"

shader::shader():shader("../shaders/default.vert","../shaders/default.frag","default"){

}

shader::shader(const char* vertexPath, const char* fragmentPath){
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        errord = true;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        errord = true;
    };
    
    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        errord = true;
    };
    
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        errord = true;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

shader::shader(const GLchar* vertexPath, const GLchar* fragmentPath, std::string name):shader(vertexPath,fragmentPath){
    shader_name = name;
}

void shader::setCamera(float pos_x, float pos_y){
    cameraVector.x = - pos_x;
    cameraVector.y = - pos_y;
}

void shader::moveCamera(float delta_pos_x, float delta_pos_y){
    cameraVector.x -= delta_pos_x;
    cameraVector.y -= delta_pos_y;
}

bool shader::hasErrored(){
    return errord;
}

unsigned int shader::getID(){
    return ID;
}

void shader::use(){
    glUseProgram(ID);

    camera = glm::mat4(1.0f);
    camera = glm::translate(camera, cameraVector); 

    int cameraLoc = glGetUniformLocation(getID(), "camera");
    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(camera));

    glm::mat4 projection = projectionMatrix;
    
    int projectionLoc = glGetUniformLocation(getID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

std::string shader::getName(){
    return shader_name;
}

void shader::setBool(const std::string &name, bool value) const{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void shader::setBool(const std::string &name, bool value, bool value2) const{ 
    glUniform2i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2); 
}

void shader::setBool(const std::string &name, bool value, bool value2, bool value3) const{ 
    glUniform3i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2, (int)value3); 
} 

void shader::setBool(const std::string &name, bool value, bool value2, bool value3, bool value4) const{ 
    glUniform4i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2, (int)value3, (int)value4); 
} 


void shader::setInt(const std::string &name, int value) const{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void shader::setInt(const std::string &name, int value, int value2) const{ 
    glUniform2i(glGetUniformLocation(ID, name.c_str()), value, value2); 
}

void shader::setInt(const std::string &name, int value, int value2, int value3) const{ 
    glUniform3i(glGetUniformLocation(ID, name.c_str()), value, value2, value3); 
} 

void shader::setInt(const std::string &name, int value, int value2, int value3, int value4) const{ 
    glUniform4i(glGetUniformLocation(ID, name.c_str()), value, value2, value3, value4); 
} 


void shader::setFloat(const std::string &name, float value) const{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
} 

void shader::setFloat(const std::string &name, float value, float value2) const{ 
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value, value2); 
}

void shader::setFloat(const std::string &name, float value, float value2, float value3) const{ 
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value, value2, value3); 
} 

void shader::setFloat(const std::string &name, float value, float value2, float value3, float value4) const{ 
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value, value2, value3, value4); 
} 

void shader::getFloat(const std::string &name, float dest[]) const{
    glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), dest);
}

void shader::setProjectionmatrix(glm::mat4 projection){
    projectionMatrix = projection;
}