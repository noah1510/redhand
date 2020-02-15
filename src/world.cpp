#include "redhand/world.hpp"

world::world(){
    WorldShaders = std::vector<shader*>(0);
    WorldTextures = std::vector<texture2D*>(0);
    WorldObjects = std::vector<game_object*>(0);

    setWindowSize(600,600);
}

world::~world(){
    WorldObjectsMutex.lock();
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
    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x : WorldObjects){
        x->onLoop(window);
    }

}

void world::draw(){
    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x : WorldObjects){
        x->draw();
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

        std::scoped_lock<std::shared_mutex> lock(WorldObjectsMutex);
        
        for(auto x:WorldObjects){
            x->setScreenSize(width,height);
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
        shade->setCamera(cameraPosition[0], cameraPosition[1]);
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

    std::scoped_lock<std::shared_mutex> lock(WorldObjectsMutex);

    WorldObjects.emplace_back(obj);
    if(WorldObjects.back() == obj){
        obj->setScreenSize(windowWidth,windowHeight);
        return 0;
    }else{
        return -3;
    }
}

int world::removeShader(std::string name){
    for(auto x:WorldShaders){
        if(x->getName().compare(name) == 0){
            try{
                delete x;
            }
            catch(const std::exception& e){
                std::cerr << e.what() << '\n';
                return -1;
            }
            return 0;
        }
    }

    return 1;    
}

int world::removeTexture(std::string name){
    for(auto x:WorldTextures){
        if(x->getName().compare(name) == 0){
            try{
                delete x;
            }
            catch(const std::exception& e){
                std::cerr << e.what() << '\n';
                return -1;
            }
            return 0;
        }
    }

    return 1; 
}

int world::removeObject(std::string name){
    std::scoped_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x:WorldObjects){
        if(x->getName().compare(name) == 0){
            try{
                delete x;
            }
            catch(const std::exception& e){
                std::cerr << e.what() << '\n';
                return -1;
            }
            return 0;
        }
    }

    return 1; 

}

shader* world::getShaderByName(std::string name){

    for(auto x:WorldShaders){
       if((x->getName()).compare(name) == 0){
            return x;  
        } 
    }

    return nullptr;
    
}

texture2D* world::getTextureByName(std::string name){

    for(auto x:WorldTextures){
       if((x->getName()).compare(name) == 0){
            return x;  
        } 
    }

    return nullptr;
    
    
}

game_object* world::getObjectByName(std::string name){
    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x:WorldObjects){
        if(x->getName().compare(name) == 0){
            return x;
        }
    }

    return nullptr;
    
}

void world::setCamera(float pos_x, float pos_y){
    cameraPosition[0] = pos_x;
    cameraPosition[1] = pos_y;

    for(auto x : WorldShaders){
        x->setCamera(cameraPosition[0], cameraPosition[1]);
    }
}

void world::moveCamera(float delta_pos_x, float delta_pos_y){
    cameraPosition[0] += delta_pos_x;
    cameraPosition[1] += delta_pos_y;

    for(auto x:WorldShaders){
        x->moveCamera(delta_pos_x, delta_pos_y);
    }  
}