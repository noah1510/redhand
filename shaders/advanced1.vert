#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform vec4 positionOffset;

out vec3 vertColor;
out vec2 TexCoord;

void main(){
    gl_Position = vec4(aPos + positionOffset.xyz, 1.0f + positionOffset.w);
    vertColor = aColor+aPos+positionOffset.xyz;
    TexCoord = aTexCoord;
}