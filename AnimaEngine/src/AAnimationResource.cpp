#include "AAnimationResource.h"
#include <iostream>

AAnimationResource::AAnimationResource(int f){
	//reserve space for meshes
	meshes.reserve(f);
}

AAnimationResource::~AAnimationResource(){
	for(int i=0;i<meshes.size();i++){
		delete meshes[i];
		meshes[i] = nullptr;
	}
	meshes.resize(0);
}

AAnimationResource::AAnimationResource(const AAnimationResource&){

}

void AAnimationResource::loadUniforms(float* m, float* v, float* p, GLuint pid, int pointer){
	meshes[pointer]->loadUniforms(m,v,p,pid);
}



bool AAnimationResource::loadResource(std::string name){
	
	//set the name to the resource
	setName(name);

	//load the meshes one per one
	for(int i=0;i<meshes.capacity();i++){

		//concatenate the file name
		std::string number = std::to_string(i);

		while(number.length()<6){
			number = "0" + number;
		}

		std::string obj = name + "_" + number + ".obj";
		
		//mesh handler
		AMeshResource* m = new AMeshResource();
		//load it
		if(!m->loadResource(obj)){
			std::cout << "Error loading animation" << "\n";
			exit(0);
		}

		meshes.push_back(m);

		m = nullptr;
	}

	return true;
	
}

void AAnimationResource::draw(){
	meshes[0]->draw();
}

void AAnimationResource::draw(int i){
	meshes[i]->draw();
}

/*-----------SETTERS------------*/
void AAnimationResource::setTextureResource(ATextureResource* tr){
	for(int i=0;i<meshes.size();i++){
		meshes[i]->setTextureResource(tr);
	}
}

void AAnimationResource::setMaterialResource(AMaterialResource* mr){
	for(int i=0;i<meshes.size();i++){
		meshes[i]->setMaterialResource(mr);
	}
}