#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec2 TexCoord;

uniform int useTexture;

uniform sampler2D tex1;

void main(){
    if(useTexture == 1){
        FragColor = texture(tex1, TexCoord);
    }else if(useTexture == 2){
        FragColor = texture(tex1, TexCoord) * vec4(vertColor, 1.0f);
    }else{
        FragColor = vec4(vertColor, 1.0f);
    }
}