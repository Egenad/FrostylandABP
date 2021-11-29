#include "ATextureResource.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION //we decided to implement this library to read various types of files
#include "stb_image.h"

ATextureResource::ATextureResource(){
	isActive = false;
}

ATextureResource::~ATextureResource(){
	glDeleteTextures(1, &tid);
}

bool ATextureResource::loadResource(std::string file){

	//********************************************************
	// NOTICE: The code to use the stb library to read images
	//		and creating the texture was taken from:
	//    https://learnopengl.com/Getting-started/Textures
	//********************************************************

    //first asign the name to the resource
    setName(file);

	int nChannels;
    //reading the file using stb library
    unsigned char *data = stbi_load(file.c_str(), &width, &height, &nChannels, STBI_rgb_alpha); 

    if(data){
    	//creating the texture for opengl
	    glGenTextures(1, &tid);

	    //bind the texture so any subsequent texture commands will configure the currently bound texture
	    glBindTexture(GL_TEXTURE_2D, tid);

	    // set the texture wrapping/filtering options (on the currently bound texture object -> GL_TEXTURE_2D)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//flip vertycally the image
		//stbi_set_flip_vertically_on_load(true);

	    //generating the texture with the loaded data
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	    //now the currently bound texture object has the texture image attached to it (GL_TEXTURE_2D)
		/*
	    1st param: texture target. This operation will generate a texture on the currently bound
	    texture object at the same target (which is GL_TEXTURE_2D)
	    2nd param: mipmap level for which we want to create a texture
	    3rd param: what kind of format we want to store the texture.
	    4th and 5th param: width and height of the image.
	    6th param: should always be 0.
	    7th and 8th param: format and datatype of the source image.
	    9th param: pixel data.
	    */
	    //generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);

		isActive = true;

		//FREE MEMORY
		stbi_image_free(data);

    }else{
    	std::cout << "Failed to load texture " << file << "\n";
    	exit(0);
    }

    return true;
}

void ATextureResource::draw(){

    if(isActive){
        //bind the texture
        glBindTexture(GL_TEXTURE_2D, tid);
    }
}

//--------------GETTERS----------------

int ATextureResource::getWidth(){
	return width;
}

int ATextureResource::getHeight(){
	return height;
}