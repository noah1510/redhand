#include "redhand/shader.hpp"

using namespace redhand;

#define VERTEX_SHADER GL_VERTEX_SHADER
#define FRAGMENT_SHADER GL_FRAGMENT_SHADER

redhand::shader::shader():shader("default"){
    createDefaultShader();
};

redhand::shader::shader(std::string_view name){
    shader_name = static_cast<std::string>(name);
}

int redhand::shader::createDefaultShader(){
    return createShaderFromCode(redhand::DefaultVertexSource,redhand::DefaultFragmentSource);
}

int redhand::shader::createShaderFromFile(std::string_view vertexPath, std::string_view fragmentPath){
    //retrieve the vertex/fragment source code from filePath
    auto vertexCode = loadShaderCodeFromFile(vertexPath);
    if(vertexCode.compare("") == 0){return -4;};

    auto fragmentCode = loadShaderCodeFromFile(fragmentPath);
    if(fragmentCode.compare("") == 0){return -5;};

    return createShaderFromCode(vertexCode, fragmentCode);
};

int redhand::shader::createShaderFromCode(std::string_view vertexSource, std::string_view fragmentSource){
    // compile shaders
    auto vertex = compileShader(static_cast<std::string>(vertexSource).c_str(),VERTEX_SHADER);
    if (vertex == 0){return -1;};

    auto fragment = compileShader(static_cast<std::string>(fragmentSource).c_str(),FRAGMENT_SHADER);
    if (fragment == 0){return -2;};

    //link shaders
    linkShader(vertex,fragment);
    
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if(initilized){
        return 0;
    }else{
        return -3;
    }
}

std::string redhand::shader::loadShaderCodeFromFile(std::string_view fileLocation){
    if(errord){return "";};

    std::string shaderCode;
    std::ifstream ShaderFile;
    // ensure ifstream objects can throw exceptions:
    ShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        ShaderFile.open(std::string(fileLocation));
        std::stringstream ShaderStream;
        // read file's buffer contents into streams
        ShaderStream << ShaderFile.rdbuf();		
        // close file handlers
        ShaderFile.close();
        // convert stream into string
        shaderCode  = ShaderStream.str();	
    }
    catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        errord = true;
        return "";
    }

    return shaderCode;
}

unsigned int redhand::shader::compileShader(std::string_view shaderSource, int shaderType){
    if(errord){return 0;};
    // compile the shader and return the location of the result
    int success;
    char infoLog[512];

    auto stringSource = std::string(shaderSource);
    auto shaderCode = stringSource.c_str();

    auto shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // print compile errors if anything went wrong
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cerr << "Shader source code:" << shaderCode << std::endl;
        errord = true;
        return 0;
    };

    return shader;
}


void redhand::shader::linkShader(unsigned int vertexShader, unsigned int fragmentShader){
    if(errord){return;};

    // create shader Program
    int success;
    char infoLog[512];

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        errord = true;
    }
    
    initilized = true;

}

void redhand::shader::setCamera(float pos_x, float pos_y){
    auto lock = {
        std::scoped_lock(mutex_cameraVector),
        std::scoped_lock(mutex_camera)
    };

    cameraVector.x = - pos_x;
    cameraVector.y = - pos_y;

    camera = glm::mat4(1.0f);
    camera = glm::translate(camera, cameraVector);
}

void redhand::shader::moveCamera(float delta_pos_x, float delta_pos_y){
    auto lock = {
        std::scoped_lock(mutex_cameraVector),
        std::scoped_lock(mutex_camera)
    };

    cameraVector.x -= delta_pos_x;
    cameraVector.y -= delta_pos_y;

    camera = glm::mat4(1.0f);
    camera = glm::translate(camera, cameraVector);
}

bool redhand::shader::hasErrored(){
    auto lock = {
        std::shared_lock(mutex_initilized),
        std::shared_lock(mutex_errord)
    };

    return !initilized || errord;
}

unsigned int shader::getID(){
    return ID;
}

void redhand::shader::use(){
    auto lock = {
        std::shared_lock(mutex_camera),
        std::shared_lock(mutex_projectionMatrix),
        std::shared_lock(mutex_textureScale)
    };

    glUseProgram(ID);

    int cameraLoc = glGetUniformLocation(getID(), "camera");
    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(camera));
    
    int projectionLoc = glGetUniformLocation(getID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    setFloat("textureScale", textureScale.x, textureScale.y);
}

std::string_view redhand::shader::getName(){
    auto lock = std::shared_lock(mutex_shader_name);
    return shader_name;
}

void redhand::shader::setProjectionmatrix(glm::mat4 projection){
    auto lock = std::scoped_lock(mutex_projectionMatrix);
    projectionMatrix = projection;
}

void redhand::shader::setTextureScale(glm::vec2 scale){
    auto lock = std::scoped_lock(mutex_textureScale);
    textureScale = scale;
}

void redhand::shader::setBool(const std::string &name, bool value) const{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void redhand::shader::setBool(const std::string &name, bool value, bool value2) const{ 
    glUniform2i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2); 
}

void redhand::shader::setBool(const std::string &name, bool value, bool value2, bool value3) const{ 
    glUniform3i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2, (int)value3); 
} 

void redhand::shader::setBool(const std::string &name, bool value, bool value2, bool value3, bool value4) const{ 
    glUniform4i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2, (int)value3, (int)value4); 
} 


void redhand::shader::setInt(const std::string &name, int value) const{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void redhand::shader::setInt(const std::string &name, int value, int value2) const{ 
    glUniform2i(glGetUniformLocation(ID, name.c_str()), value, value2); 
}

void redhand::shader::setInt(const std::string &name, int value, int value2, int value3) const{ 
    glUniform3i(glGetUniformLocation(ID, name.c_str()), value, value2, value3); 
} 

void redhand::shader::setInt(const std::string &name, int value, int value2, int value3, int value4) const{ 
    glUniform4i(glGetUniformLocation(ID, name.c_str()), value, value2, value3, value4); 
} 


void redhand::shader::setFloat(const std::string &name, float value) const{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
} 

void redhand::shader::setFloat(const std::string &name, float value, float value2) const{ 
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value, value2); 
}

void redhand::shader::setFloat(const std::string &name, float value, float value2, float value3) const{ 
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value, value2, value3); 
} 

void redhand::shader::setFloat(const std::string &name, float value, float value2, float value3, float value4) const{ 
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value, value2, value3, value4); 
} 

void redhand::shader::getFloat(const std::string &name, float dest[]) const{
    glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), dest);
}