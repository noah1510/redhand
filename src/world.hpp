#pragma once

#include <vector>

#include "objects.hpp"

class world{
private:
    std::vector <object*> WorldObjects;
    std::vector <shader*> WorldShaders;
    std::vector <texture2D*> WorldTextures;
public:

    world();
    ~world();

    int addShader(shader*);
    int addTexture(texture2D*);
    int addObject(object*);

    int removeShader(int);
    int removeTexture(int);
    int removeObject(int);

    shader* getShaderByIndex(int);
    texture2D* getTextureByIndex(int);
    object* getObjectByIndex(int);

    shader* getShaderByName(std::string name);

    void tick(GLFWwindow* window);
    int test();
};