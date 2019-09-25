#version 330 core 
out vec4 FragColor;
in vec2 TexCoord;

uniform int useTexture;
uniform vec4 ourColor;
uniform sampler2D tex1;

void main(){
    if(useTexture == 1){
        FragColor = texture(tex1, TexCoord);
    }else if(useTexture == 2){
        FragColor = texture(tex1, TexCoord) * ourColor;
    }else{
        FragColor = ourColor;
    }
}