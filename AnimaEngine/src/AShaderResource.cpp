#include "AShaderResource.h"
#include <iostream>
#include <glew.h>
#include <gl.h>
#include <glfw3.h>

AShaderResource::AShaderResource(){
	ID = -1;
}

AShaderResource::~AShaderResource(){

}

bool AShaderResource::loadResource(std::string file){

	//********************************************************
	// NOTICE: The code for reading and compiling the shaders
	// 					was taken from:
	//    https://learnopengl.com/Getting-started/Shaders 
	//********************************************************

    //first asign the name to the resource
    setName(file);

	//strings to store the shader's code
    std::string vertexCode;
    std::string fragmentCode;
    //reading buffers
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    //making sure that ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {

    	std::string vertexpath, fragmentpath;
    	vertexpath = file + ".vs";
    	fragmentpath = file + ".fs";

        //open files
        vShaderFile.open(vertexpath.c_str());
        fShaderFile.open(fragmentpath.c_str());

        // read file's buffer contents into streams
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();	

        //close file handlers
        vShaderFile.close();
        fShaderFile.close();

        //convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }catch (std::ifstream::failure e){
    	//if there is any problem reading the shader files, show error message and close the program
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        exit(0);
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    //link vertex and shader to create the program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success;
    char infoLog[512];

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(0);
    }
    checkCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

void AShaderResource::draw(){
	//unused method
}

void AShaderResource::use(){
	//telling opengl that we're using this shader program
    //std::cout << "Using program " << ID << std::endl;
	glUseProgram(ID);
}

void AShaderResource::checkCompileErrors(unsigned int shader, std::string type){

	//compile the shaders and show message errors

	int success;
    char infoLog[1024];
    if (type != "PROGRAM"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

unsigned int AShaderResource::getProgramID(){
	return ID;
}