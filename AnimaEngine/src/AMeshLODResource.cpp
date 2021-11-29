#include "AMeshLODResource.h"
#include <iostream>

AMeshLODResource::AMeshLODResource(int f){
	//reserve space for meshes
	meshes.reserve(f);
}

AMeshLODResource::~AMeshLODResource(){
	for(int i=0;i<meshes.size();i++){
		delete meshes[i];
		meshes[i] = nullptr;
	}
	meshes.resize(0);
}

void AMeshLODResource::loadUniforms(float* m, float* v, float* p, GLuint pid, int pointer){
	meshes[pointer]->loadUniforms(m,v,p,pid);
}



bool AMeshLODResource::loadResource(std::string name){
    	//set the name to the resource
	setName(name);
	//load the meshes one per one
	for(int i=0;i<meshes.capacity();i++){
		//concatenate the file name
		std::string number = std::to_string(i);
		number = "0" + number;

		std::string obj = name + "_" + number + ".obj";
		//mesh handler
		AMeshResource* m = new AMeshResource();
		//load it
		if(!m->loadResource(obj)){
			std::cout << "Error loading LOD meshes" << "\n";
			exit(0);
		}
		meshes.push_back(m);
		m = nullptr;
	}
	return true;
}

void AMeshLODResource::draw(){
	meshes[0]->draw();
}

void AMeshLODResource::draw(int i){
	meshes[i]->draw();
}

/*-----------SETTERS------------*/
void AMeshLODResource::setTextureResource(int i, ATextureResource* tr){
	meshes[i]->setTextureResource(tr);
}

void AMeshLODResource::setMaterialResource(AMaterialResource* mr){
	for(int i=0;i<meshes.size();i++){
		meshes[i]->setMaterialResource(mr);
	}
}