#include "world.hpp"

world::world(){
    WorldShaders = std::vector<shader*>(0);
    WorldTextures = std::vector<texture2D*>(0);
    WorldObjects = std::vector<game_object*>(0);

    setWindowSize(600,600);
}

world::~world(){
    //delete objects
    try{
        for(int i = 0;i < WorldObjects.size();i++){
            delete WorldObjects.at(i);
        }
        WorldObjects.clear();
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }

    //delte textures
    try{
        for(int i = 0;i < WorldTextures.size();i++){
            delete WorldTextures.at(i);
        }
        WorldTextures.clear();
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }

    //delete shaders
    try{
        for(int i = 0;i < WorldShaders.size();i++){
            delete WorldShaders.at(i);
        }
        WorldShaders.clear();
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }
    
    
}

void world::tick(GLFWwindow* window){

    for(int i = 0; i < WorldObjects.size(); i++){
        WorldObjects.at(i)->onLoop(window);
        WorldObjects.at(i)->draw();
    }

}

void world::setWindowSize(int width, int height){

    if(width != windowWidth || height != windowHeight){
        windowWidth = width;
        windowHeight = height;

        projectionMatrix = glm::ortho(-1.0f, (float) width / (float) height, -1.0f, (float) height / (float) width, -10.0f, 10.0f);

        for(int i = 0;i < WorldShaders.size();i++){
            WorldShaders.at(i)->setProjectionmatrix(projectionMatrix);
        }

        for(int i = 0;i < WorldObjects.size();i++){
            WorldObjects.at(i)->setScreenSize(width,height);
        }
    }

}

int world::addShader(shader* shade){
    if(shade == nullptr || shade == NULL){
        return -1;
    }

    if(shade->hasErrored()){
        return -2;
    }

    WorldShaders.emplace_back(shade);
    if(WorldShaders.back() == shade){
        shade->setProjectionmatrix(projectionMatrix);
        return 0;
    }else{
        return -3;
    }
}

int world::addTexture(texture2D* tex){
    if(tex == nullptr || tex == NULL){
        return -1;
    }

    if(tex->hasErrord()){
        return -2;
    }

    WorldTextures.emplace_back(tex);
    if(WorldTextures.back() == tex){
        return 0;
    }else{
        return -3;
    }
}

int world::addObject(game_object* obj){
    if(obj == nullptr || obj == NULL){
        return -1;
    }

    if(obj->hasErrord()){
        return -2;
    }

    WorldObjects.emplace_back(obj);
    if(WorldObjects.back() == obj){
        obj->setScreenSize(windowWidth,windowHeight);
        return 0;
    }else{
        return -3;
    }
}

int world::removeShader(shader* shade){
    for(int i = 0; i < WorldShaders.size();i++){
        if(shade == WorldShaders.at(i)){
            WorldShaders.erase(WorldShaders.begin() + i);
            try{
                delete shade;
            }
            catch(const std::exception& e){
                std::cout << e.what() << '\n';
                return -1;
            }
            return 0;
        }   
    }

    return 1;    
}

int world::removeTexture(texture2D* tex){
    for(int i = 0; i < WorldTextures.size();i++){
        if(tex == WorldTextures.at(i)){
            WorldTextures.erase(WorldTextures.begin() + i);
            try{
                delete tex;
            }
            catch(const std::exception& e){
                std::cout << e.what() << '\n';
                return -1;
            }
            return 0;
        }   
    }

    return 1;
}

int world::removeObject(game_object* obj){
    for(int i = 0; i < WorldObjects.size();i++){
        if(obj == WorldObjects.at(i)){
            WorldObjects.erase(WorldObjects.begin() + i);
            try{
                delete obj;
            }
            catch(const std::exception& e){
                std::cout << e.what() << '\n';
                return -1;
            }
            return 0;
        }   
    }
    return 1;
}

shader* world::getShaderByName(std::string name){
    int i = 0;
    bool found = false;

    while(i < WorldShaders.size()){
        if((WorldShaders.at(i)->getName()).compare(name) == 0){
            found = true;
            break;    
        }
        i++;
    }

    if(found){
        return WorldShaders.at(i);
    }else{
        return nullptr;
    }
    
}

texture2D* world::getTextureByName(std::string name){
    int i = 0;
    bool found = false;
    while(i < WorldTextures.size()){
        if((WorldTextures.at(i)->getName()).compare(name) == 0){
            found = true;
            break;    
        }
        i++;
    }

    if(found){
        return WorldTextures.at(i);
    }else{
        return nullptr;
    }
    
}

game_object* world::getObjectByName(std::string name){
    int i = 0;
    bool found = false;

    while(i < WorldObjects.size()){
        if((WorldObjects.at(i)->getName()).compare(name) == 0){
            found = true;
            break;    
        }
        i++;
    }

    if(found){
        return WorldObjects.at(i);
    }else{
        return nullptr;
    }
    
}

void world::setCamera(float pos_x, float pos_y){
    cameraPosition[0] = pos_x;
    cameraPosition[1] = pos_y;

    for(int i = 0;i < WorldShaders.size();i++){
        WorldShaders.at(i)->setCamera(cameraPosition[0], cameraPosition[1]);
    }
}

void world::moveCamera(float delta_pos_x, float delta_pos_y){
    cameraPosition[0] += delta_pos_x;
    cameraPosition[1] += delta_pos_y;

    for(int i = 0;i < WorldShaders.size();i++){
        WorldShaders.at(i)->moveCamera(delta_pos_x, delta_pos_y);
    }    
}