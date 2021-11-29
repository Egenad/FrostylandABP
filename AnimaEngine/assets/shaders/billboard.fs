#version 400 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{

    FragColor = texture(ourTexture, TexCoord);

    if(FragColor.a == 0.0){
    	discard;
    }

}