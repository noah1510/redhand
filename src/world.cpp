#include "redhand/world.hpp"

using namespace redhand;

redhand::world::world(){
    WorldShaders = std::vector< std::shared_ptr<redhand::shader>>(0);
    WorldTextures = std::vector< std::shared_ptr<redhand::texture2D>>(0);
    WorldObjects = std::vector< std::shared_ptr<redhand::game_object>>(0);

    setWindowSize(600,600);
}

redhand::world::~world(){
    WorldObjectsMutex.lock();
    //delete objects
    try{
        for(auto x:WorldObjects){
            if (x.use_count() > 1){
                std::cout << "still having references:" << x.use_count() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
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
            if (x.use_count() > 1){
                std::cout << "still having references:" << x.use_count() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
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
            if (x.use_count() > 1){
                std::cout << "still having references:" << x.use_count() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            x.reset();
        }
        WorldShaders.clear();
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }
    
}

void redhand::world::tick(GLFWwindow* window){
    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x : WorldObjects){
        x->onLoop(window);
    }

}

void redhand::world::draw(){
    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x : WorldObjects){
        x->draw();
    }

}

void redhand::world::setWindowSize(int width, int height){

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

int redhand::world::addShader( std::unique_ptr<redhand::shader> shade){
    if(shade == nullptr || shade == NULL){
        return -1;
    }

    if(shade->hasErrored()){
        return -2;
    }

    auto local = std::move(shade);

    WorldShaders.emplace_back( std::shared_ptr<redhand::shader>(local.release()));
    if(WorldShaders.back() != nullptr){
        shade->setProjectionmatrix(projectionMatrix);
        shade->setCamera(cameraPosition[0], cameraPosition[1]);
        return 0;
    }else{
        return -4;
    }
}

int redhand::world::addTexture(std::unique_ptr<redhand::texture2D> tex){
    if(tex == nullptr || tex == NULL){
        return -1;
    }

    if(tex->hasErrord()){
        return -2;
    }

    auto local = std::move(tex);

    WorldTextures.emplace_back(std::shared_ptr<redhand::texture2D>(local.release()));
    if(WorldTextures.back() != nullptr){
        return 0;
    }else{
        return -3;
    }
}

int redhand::world::addObject(std::unique_ptr<game_object> obj){
    if(obj == nullptr || obj == NULL){
        return -1;
    }

    if(obj->hasErrord()){
        return -2;
    }

    std::scoped_lock<std::shared_mutex> lock(WorldObjectsMutex);

    WorldObjects.emplace_back(std::shared_ptr<redhand::game_object>(std::move(obj)));
    if(WorldObjects.back() != nullptr){
        obj->setScreenSize(windowWidth,windowHeight);
        return 0;
    }else{
        return -3;
    }
}

int redhand::world::removeShader(std::string name){
    for(unsigned int i = 0; i < WorldShaders.size();i++){
        auto x = WorldShaders.at(i);
        if(x->getName().compare(name) == 0){
            try{
                WorldShaders.erase(WorldShaders.begin() + i);
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

int redhand::world::removeTexture(std::string name){
    for(unsigned int i = 0; i < WorldTextures.size();i++){
        auto x = WorldTextures.at(i);
        if(x->getName().compare(name) == 0){
            try{
                WorldTextures.erase(WorldTextures.begin() + i);
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

int redhand::world::removeObject(std::string name){
    std::scoped_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(unsigned int i = 0; i < WorldObjects.size();i++){
        auto x = WorldObjects.at(i);
        if(x->getName().compare(name) == 0){
            try{
                WorldObjects.erase(WorldObjects.begin() + i);
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

 std::shared_ptr<redhand::shader> redhand::world::getShaderByName(std::string name){

    for(auto x:WorldShaders){
       if((x->getName()).compare(name) == 0){
            return  std::shared_ptr<redhand::shader>(x);  
        } 
    }

    return nullptr;
    
}

std::shared_ptr<redhand::texture2D> redhand::world::getTextureByName(std::string name){

    for(auto x:WorldTextures){
       if((x->getName()).compare(name) == 0){
            return std::shared_ptr<redhand::texture2D>(x);  
        } 
    }

    return nullptr;
    
    
}

std::shared_ptr<redhand::game_object> redhand::world::getObjectByName(std::string name){
    std::shared_lock<std::shared_mutex> lock(WorldObjectsMutex);

    for(auto x:WorldObjects){
        if(x->getName().compare(name) == 0){
            return std::shared_ptr<redhand::game_object>(x);
        }
    }

    return nullptr;
    
}

void redhand::world::setCamera(float pos_x, float pos_y){
    cameraPosition[0] = pos_x;
    cameraPosition[1] = pos_y;

    for(auto x : WorldShaders){
        x->setCamera(cameraPosition[0], cameraPosition[1]);
    }
}

void redhand::world::moveCamera(float delta_pos_x, float delta_pos_y){
    cameraPosition[0] += delta_pos_x;
    cameraPosition[1] += delta_pos_y;

    for(auto x:WorldShaders){
        x->moveCamera(delta_pos_x, delta_pos_y);
    }  
}