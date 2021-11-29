#version 400 core

uniform vec4 v_Color;

out vec4 FragColor;

void main()
{
    FragColor = v_Color;
}