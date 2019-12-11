#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec2 TexCoord;

uniform int textureMode;
uniform float colorAlpha;

uniform vec2 textureScale;

uniform sampler2D tex1;

void main(){
    if(textureMode == 1){
        FragColor = texture(tex1, vec2(textureScale.x * TexCoord.x,textureScale.y * TexCoord.y)) * vec4(vertColor, colorAlpha);
    }else{
        FragColor = vec4(vertColor, colorAlpha);
    }
}