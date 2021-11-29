#version 400 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D text;

void main()
{    

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, TexCoord).r);
    FragColor = vec4(1.0, 0.0, 0.0, 1.0) * sampled;
}