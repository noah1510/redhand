#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertColor;
out vec2 TexCoord;

uniform mat4 worldTransformation;
uniform mat4 camera;

void main(){
    gl_Position = camera * worldTransformation * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    vertColor = aColor;
    TexCoord = aTexCoord;
}