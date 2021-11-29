#version 400 core
layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 TextureCoords;


out vec2 TexCoord;
out vec3 vNormal;
out vec4 vPosition;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{

    TexCoord = TextureCoords;
    vPosition = ModelMatrix * VertexPosition;
    vNormal = mat3(transpose(inverse(ModelMatrix)))* VertexNormal;
    gl_Position = ProjectionMatrix * ViewMatrix * vPosition;
}