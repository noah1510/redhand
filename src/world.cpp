#include "world.hpp"

world::world(){
    WorldShaders = std::vector<shader*>(0);
    WorldTextures = std::vector<texture2D*>(0);
    WorldObjects = std::vector<object*>(0);
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

int world::test(){
    try{
        int ssize = WorldShaders.size();
        int tsize = WorldTextures.size();
        int osize = WorldObjects.size();

        if(ssize <= 0 || tsize <= 0 || osize <= 0){
            return -2;
        }

        if(WorldObjects.at(0) == NULL || WorldObjects.at(0) == nullptr){
            return -3;
        }

        return 0;
    }catch(std::exception e){
        std::cout << e.what() << std::endl;
        return -1;
    }
}

int world::addShader(shader* shade){
    WorldShaders.emplace_back(shade);
    if(WorldShaders.back() == shade){
        return 0;
    }else{
        return -1;
    }
}

int world::addTexture(texture2D* tex){
    WorldTextures.emplace_back(tex);
    if(WorldTextures.back() == tex){
        return 0;
    }else{
        return -1;
    }
}

int world::addObject(object* obj){
    WorldObjects.emplace_back(obj);
    if(WorldObjects.back() == obj){
        return 0;
    }else{
        return -1;
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

int world::removeObject(object* obj){
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

shader* world::getShaderByIndex(int i){
    try{
        return WorldShaders.at(i);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return nullptr;
    }
}

texture2D* world::getTextureByIndex(int i){
    try{
        return WorldTextures.at(i);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return nullptr;
    }     
}

object* world::getObjectByIndex(int i){
    try{
        return WorldObjects.at(i);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return nullptr;
    }
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