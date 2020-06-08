#include <redhand/glad/glad.h>

#include "redhand/complex_world.hpp"
#include "redhand/engine.hpp"
#include "redhand/game_object.hpp"
#include "redhand/shader.hpp"
#include "redhand/texture.hpp"

using namespace redhand;

redhand::complex_world::complex_world(){
    WorldShaders = std::vector< std::shared_ptr<redhand::shader>>(0);
    WorldTextures = std::vector< std::shared_ptr<redhand::texture2D>>(0);
    WorldObjects = std::vector< std::shared_ptr<redhand::game_object>>(0);

    setWindowSize(600,600);
}

redhand::complex_world::~complex_world(){
    WorldObjectsMutex.lock();
    //delete objects
    try{
        for(auto x:WorldObjects){
            x.reset();
        }
        WorldObjects.clear();
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }

    //delte textures
    try{
        for(auto x:WorldTextures){
            x.reset();
        }
        WorldTextures.clear();
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }

    //delete shaders
    try{
        for(auto x:WorldShaders){
            x.reset();
        }
        WorldShaders.clear();
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }
    
}

void redhand::complex_world::tick(redhand::game_loop_event evt){
    
    auto window = evt.getRaiser()->getWindow();

    //update window size
    int width = 0, height = 0;
    glfwGetWindowSize(window, &width, &height);
    setWindowSize(width, height);

    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);
    for(unsigned int i = 0;i < WorldObjects.size();i++){
        WorldObjects.at(i)->onLoop(window);
    }

}

void redhand::complex_world::draw(){
    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x : WorldObjects){
        x->draw();
    }

}

void redhand::complex_world::setWindowSize(int width, int height){

    if(width != windowWidth || height != windowHeight){
        windowWidth = width;
        windowHeight = height;

        projectionMatrix = glm::ortho(-1.0f, (float) width / (float) height, -1.0f, (float) height / (float) width, -10.0f, 10.0f);

        for(unsigned int i = 0;i < WorldShaders.size();i++){
            WorldShaders.at(i)->setProjectionmatrix(projectionMatrix);
        }

        std::scoped_lock<std::shared_mutex> lock(WorldObjectsMutex);
        
        for(auto x:WorldObjects){
            x->setScreenSize(width,height);
        }
    }

}

int redhand::complex_world::addShader( std::unique_ptr<redhand::shader> shade){
    if(shade == nullptr || shade == NULL){
        return -1;
    }

    if(shade->hasErrored()){
        return -2;
    }

    WorldShaders.emplace_back( std::shared_ptr<redhand::shader>(std::move(shade)));
    if(WorldShaders.back() != nullptr){
        WorldShaders.back()->setProjectionmatrix(projectionMatrix);
        WorldShaders.back()->setCamera(cameraPosition[0], cameraPosition[1]);
        return 0;
    }else{
        return -4;
    }
}

int redhand::complex_world::addTexture(std::unique_ptr<redhand::texture2D> tex){
    if(tex == nullptr || tex == NULL){
        return -1;
    }

    if(tex->hasErrord()){
        return -2;
    }

    WorldTextures.emplace_back(std::shared_ptr<redhand::texture2D>(std::move(tex)));
    if(WorldTextures.back() != nullptr){
        return 0;
    }else{
        return -3;
    }
}

int redhand::complex_world::addObject(std::unique_ptr<game_object> obj){
    if(obj == nullptr || obj == NULL){
        return -1;
    }

    if(obj->hasErrord()){
        return -2;
    }

    std::scoped_lock<std::shared_mutex> lock(WorldObjectsMutex);

    WorldObjects.emplace_back(std::shared_ptr<redhand::game_object>(std::move(obj)));
    if(WorldObjects.back() != nullptr){
        WorldObjects.back()->setScreenSize(windowWidth,windowHeight);
        return 0;
    }else{
        return -3;
    }
}

int redhand::complex_world::removeShader(std::string name){
    for(unsigned int i = 0; i < WorldShaders.size();i++){
        auto x = WorldShaders.at(i);
        if(x->getName().compare(name) == 0){
            try{
                WorldShaders.erase(WorldShaders.begin() + i);
                break;
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

int redhand::complex_world::removeTexture(std::string name){
    for(unsigned int i = 0; i < WorldTextures.size();i++){
        auto x = WorldTextures.at(i);
        if(x->getName().compare(name) == 0){
            try{
                WorldTextures.erase(WorldTextures.begin() + i);
                break;
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

int redhand::complex_world::removeObject(std::string name){
    std::scoped_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(unsigned int i = 0; i < WorldObjects.size();i++){
        auto x = WorldObjects.at(i);
        if(x->getName().compare(name) == 0){
            try{
                WorldObjects.erase(WorldObjects.begin() + i);
                break;
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

 std::shared_ptr<redhand::shader> redhand::complex_world::getShaderByName(std::string name){

    for(auto x:WorldShaders){
       if((x->getName()).compare(name) == 0){
            return  std::shared_ptr<redhand::shader>(x);  
        } 
    }

    return nullptr;
    
}

std::shared_ptr<redhand::texture2D> redhand::complex_world::getTextureByName(std::string name){

    for(auto x:WorldTextures){
       if((x->getName()).compare(name) == 0){
            return std::shared_ptr<redhand::texture2D>(x);  
        } 
    }

    return nullptr;
    
    
}

std::shared_ptr<redhand::game_object> redhand::complex_world::getObjectByName(std::string name){
    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x:WorldObjects){
        if(x->getName().compare(name) == 0){
            return std::shared_ptr<redhand::game_object>(x);
        }
    }

    return nullptr;
    
}

void redhand::complex_world::setCamera(float pos_x, float pos_y){
    cameraPosition[0] = pos_x;
    cameraPosition[1] = pos_y;

    for(auto x : WorldShaders){
        x->setCamera(cameraPosition[0], cameraPosition[1]);
    }
}

void redhand::complex_world::moveCamera(float delta_pos_x, float delta_pos_y){
    cameraPosition[0] += delta_pos_x;
    cameraPosition[1] += delta_pos_y;

    for(auto x:WorldShaders){
        x->moveCamera(delta_pos_x, delta_pos_y);
    }  
}