#include "AMeshLOD.h"
#include "AMeshLODResource.h"
#include <iostream>

AMeshLOD::AMeshLOD(){

	meshes = nullptr;
	silhouette = true;
    active = 1;
}

AMeshLOD::~AMeshLOD(){
    meshes = nullptr;
}

bool AMeshLOD::loadResource(std::string name){
    return loadResource(name);
}

void AMeshLOD::beginDraw(){
	if(meshes){

		//pass the matrices to the shader
		glm::mat4 m, v, p;
		v = inverse(viewMatrix());
		m = modelMatrix();
		p = projectionMatrix();
		meshes->loadUniforms(&m[0][0], &v[0][0], &p[0][0], programID, active);

		GLuint loc = glGetUniformLocation(programID, "redraw");
		glUniform1i(loc, silhouette);

		//and draw the mesh
    	meshes->draw(active);
	}
}

void AMeshLOD::endDraw(){}

void AMeshLOD::setProgramId(GLuint pid){
	programID = pid;
}

GLuint AMeshLOD::getProgramId() const{
	return programID;
}

void AMeshLOD::setMeshLODResource(AMeshLODResource* mr){
	meshes = mr;
}

void AMeshLOD::setTextureResource(ATextureResource* t){
	//meshes->setTextureResource(t);
}

void AMeshLOD::setMaterialResource(AMaterialResource* m){
	meshes->setMaterialResource(m);
}

void AMeshLOD::setMeshSilhouette(bool s){
	silhouette = s;
}

void AMeshLOD::setMeshLODTexture(int i , ATextureResource* rt){
    meshes->setTextureResource(i, rt);
}

void AMeshLOD::setActive(int i){
    if(i >= 0 && i < 2){
        active = i;
    }
}

int AMeshLOD::getActive(){
    return active;
}
/*AMeshResource* AMeshLOD::getMeshResource() const{}

ATextureResource* AMeshLOD::getTextureResource() const{}

AMaterialResource* AMeshLOD::getMaterialResource() const{}*/