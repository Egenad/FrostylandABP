#include "AMaterialResource.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <iostream>

AMaterialResource::AMaterialResource(){
	kglossiness = -1;
	progID = -1;
}

AMaterialResource::~AMaterialResource(){
	free(kambient);
	free(kspecular);
	free(kdiffuse);
}

bool AMaterialResource::loadResource(std::string name){
	bool success = false;

	//first asign the name to the resource
	setName(name);

	Assimp::Importer i;
	const aiScene* obj = i.ReadFile(name, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(obj!=nullptr && obj->HasMaterials()){

		int nMeshes = obj->mNumMeshes;

		//Reserve memory for the components (they are vec3 of floats)
		kambient = (float*)malloc(sizeof(float) * 3);
		kspecular = (float*)malloc(sizeof(float) * 3);
		kdiffuse = (float*)malloc(sizeof(float) * 3);

		if(nMeshes > 0){
			
			aiMesh *mesh = obj->mMeshes[0];
			aiMaterial *mat = obj->mMaterials[mesh->mMaterialIndex];
			
			aiColor3D color;

			//Ambient component
			mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
			kambient[0] = color.r;
			kambient[1] = color.g;
			kambient[2] = color.b;

			//Diffuse component
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			kdiffuse[0] = color.r;
			kdiffuse[1] = color.g;
			kdiffuse[2] = color.b;

			//Specular component
			mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
			kspecular[0] = color.r;
			kspecular[1] = color.g;
			kspecular[2] = color.b;

			mat->Get(AI_MATKEY_SHININESS, kglossiness);

		}

		success = true;
	}

	return success;
}

void AMaterialResource::draw(){

	if(progID != -1){
	    GLuint kdiffuseID = glGetUniformLocation(progID, "material.kdiffuse");
	    GLuint kambientID = glGetUniformLocation(progID, "material.kambient");
	    GLuint kspecularID = glGetUniformLocation(progID, "material.kspecular");
	    GLuint kglossinessID = glGetUniformLocation(progID, "material.kglossiness");

	    glUniform3fv(kdiffuseID, 1, kdiffuse);
	    glUniform3fv(kambientID, 1, kambient);
	    glUniform3fv(kspecularID, 1, kspecular);
	    glUniform1f(kglossinessID, kglossiness);
	}
}

void AMaterialResource::setProgramID(unsigned int pid){
	progID = pid;
}