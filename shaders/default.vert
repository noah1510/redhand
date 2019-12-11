#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertColor;
out vec2 TexCoord;

uniform mat4 worldTransformation;
uniform mat4 camera;
uniform mat4 projection;

void main(){
    gl_Position = projection * camera * worldTransformation * vec4(aPos.xyz, 1.0f);
    vertColor = aColor;
    TexCoord = aTexCoord;
}