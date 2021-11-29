#include "AResourceManager.h"
#include <cstring> //strcmp
#include <iostream>

AResourceManager::AResourceManager(){
	//reserve memory for the arrays (estimated memory)

}

AResourceManager::~AResourceManager(){
	clearResourceManager();

	for(int i=0;i<shaders.size();i++){
		delete shaders[i];
		shaders[i] = nullptr;
	}

}

void AResourceManager::clearResourceManager(){

	for(int i=0;i<animations.size();i++){
		delete animations[i];
		animations[i]=nullptr;
	}

	animations.resize(0);

	for(int i=0;i<meshes.size();i++){
		delete meshes[i];
		meshes[i] = nullptr;
	}

	meshes.resize(0);

	for(int i=0;i<textures.size();i++){
		delete textures[i];
		textures[i] = nullptr;
	}

	textures.resize(0);

	for(int i=0;i<materials.size();i++){
		delete materials[i];
		materials[i] = nullptr;
	}

	materials.resize(0);

	for(int i=0;i<meshesLOD.size();i++){
		delete meshesLOD[i];
		meshesLOD[i] = nullptr;
	}

	meshesLOD.resize(0);
}

//------------GET MESH
AMeshResource* AResourceManager::getMeshResource(std::string n){

	AMeshResource* r = nullptr;
	//search the resource
	for(int i=0;i<meshes.size();i++){
		std::string name = meshes[i]->getName();

		if(strcmp(name.c_str(),n.c_str()) == 0){
			r = meshes[i];
			break;
		}
	}

	if(r == nullptr){ 
		//create the resource
		r = new AMeshResource();
		//load it
		if(!r->loadResource(n)){
			std::cout << "Error loading mesh" << "\n";
			exit(0);
		}
		//add it to our vector
		meshes.push_back(r);
	}
	return r;
}

//------------GET MESH LOD
AMeshLODResource* AResourceManager::getMeshLODResource(std::string n){

	AMeshLODResource* r = nullptr;
	//search the resource
	for(int i=0;i<meshesLOD.size();i++){
		std::string name = meshesLOD[i]->getName();

		if(strcmp(name.c_str(),n.c_str()) == 0){
			r = meshesLOD[i];
			break;
		}
	}

	if(r == nullptr){ 
		//create the resource
		r = new AMeshLODResource(2);
		//load it
		if(!r->loadResource(n)){
			std::cout << "Error loading LOD mesh" << "\n";
			exit(0);
		}
		//add it to our vector
		meshesLOD.push_back(r);
	}
	return r;
}

//---------GET SHADER
AShaderResource* AResourceManager::getShaderResource(std::string n){

	AShaderResource* r = nullptr;
	//search the resource
	for(int i=0;i<shaders.size();i++){
		std::string name = shaders[i]->getName();

		if(strcmp(name.c_str(),n.c_str()) == 0){
			r = shaders[i];
			break;
		}
	}

	if(r == nullptr){
		//create the resource
		r = new AShaderResource();
		//load it
		if(!r->loadResource(n)){
			std::cout << "Error loading shader" << "\n";
			exit(0);
		}
		//add it to our vector
		shaders.push_back(r);
	}
	return r;
}

//---------GET TEXTURE
ATextureResource* AResourceManager::getTextureResource(std::string n){

	ATextureResource* r = nullptr;
	//search the resource
	for(int i=0;i<textures.size();i++){
		std::string name = textures[i]->getName();

		if(strcmp(name.c_str(),n.c_str()) == 0){
			r = textures[i];
			break;
		}
	}

	if(r == nullptr){ 
		//create the resource
		r = new ATextureResource();
		//load it
		if(!r->loadResource(n)){
			std::cout << "Error loading texture" << "\n";
			exit(0);
		}
		//add it to our vector
		textures.push_back(r);
	}
	return r;
}

//---------GET MATERIAL
AMaterialResource* AResourceManager::getMaterialResource(std::string n){

	AMaterialResource* r = nullptr;
	//search the resource
	for(int i=0;i<materials.size();i++){
		std::string name = materials[i]->getName();

		if(strcmp(name.c_str(),n.c_str()) == 0){
			r = materials[i];
			break;
		}
	}

	if(r == nullptr){ 
		//create the resource
		r = new AMaterialResource();
		//load it
		if(!r->loadResource(n)){
			std::cout << "Error loading material" << "\n";
			exit(0);
		}
		//add it to our vector
		materials.push_back(r);
	}
	return r;
}

//---------GET ANIMATION
AAnimationResource* AResourceManager::getAnimationResource(std::string n, int fr){

	AAnimationResource* r = nullptr;
	//search the resource
	for(int i=0;i<animations.size();i++){
		std::string name = animations[i]->getName();

		if(strcmp(name.c_str(),n.c_str()) == 0){
			r = animations[i];
			break;
		}
	}

	if(r == nullptr){ 
		//create the resource
		r = new AAnimationResource(fr);
		//load it
		if(!r->loadResource(n)){
			std::cout << "Error loading animation" << "\n";
			exit(0);
		}
		//add it to our vector
		animations.push_back(r);
	}
	return r;
}


//------FREE RESOURCES FROM MEMORY
void AResourceManager::eraseMeshResource(AMeshResource* me){
	int i;
	for(i=0;i<meshes.size();i++){
		if(meshes[i]==me) break;
	}
	delete meshes[i];
	meshes[i] = nullptr;
	meshes.erase(meshes.begin()+i);
}

void AResourceManager::eraseTextureResource(ATextureResource* t){
	int i;
	for(i=0;i<textures.size();i++){
		if(textures[i]==t) break;
	}
	delete textures[i];
	textures[i] = nullptr;
	textures.erase(textures.begin()+i);
}

void AResourceManager::eraseMaterialResource(AMaterialResource* ma){
	int i;
	for(i=0;i<materials.size();i++){
		if(materials[i]==ma) break;
	}
	delete materials[i];
	materials[i] = nullptr;
	materials.erase(materials.begin()+i);
}