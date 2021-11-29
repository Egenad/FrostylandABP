#version 400 core
layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec2 TextureCoords;

out vec2 TexCoord;

void main()
{
	TexCoord = TextureCoords;
    gl_Position = VertexPosition;
}  