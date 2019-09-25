#include "world.hpp"

world::world(){
    WorldShaders = std::vector<shader*>(0);
    WorldTextures = std::vector<texture2D*>(0);
    WorldObjects = std::vector<object*>(0);
}

world::~world(){
    try{ 
        WorldShaders.clear();
        WorldTextures.clear();
        WorldObjects.clear();
    }catch(std::exception e){
        std::cout << e.what() << std::endl;
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

shader* world::getShader(int i){
    try{
        return WorldShaders.at(i);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return nullptr;
    }
}

texture2D* world::getTexture(int i){
    try{
        return WorldTextures.at(i);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return nullptr;
    }     
}

object* world::getObject(int i){
    try{
        return WorldObjects.at(i);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return nullptr;
    }
}

