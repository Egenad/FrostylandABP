#include "AMesh.h"
#include <iostream>

AMesh::AMesh(){
	mesh = nullptr;
	silhouette = true;
}

AMesh::~AMesh(){
    mesh = nullptr;
}

AMesh::AMesh(const AMesh& am){
    programID = am.getProgramId();
    mesh = new AMeshResource(*am.getMeshResource());
}


void AMesh::loadMesh(std::string fichero){
    mesh->loadResource(fichero);
}

void AMesh::beginDraw(){
	if(mesh){

		//pass the matrices to the shader
		glm::mat4 m, v, p;
		v = inverse(viewMatrix());
		m = modelMatrix();
		p = projectionMatrix();
		mesh->loadUniforms(&m[0][0], &v[0][0], &p[0][0], programID);

		GLuint loc = glGetUniformLocation(programID, "redraw");
		glUniform1i(loc, silhouette);

		//set the id for the material
		mesh->setMaterialID(programID);
		
		//and draw the mesh
    	mesh->draw();
	}
}

void AMesh::endDraw(){
    //vacio
}

void AMesh::setProgramId(GLuint pid){
	programID = pid;
}

GLuint AMesh::getProgramId() const{
	return programID;
}

void AMesh::setMeshResource(AMeshResource* mr){
	mesh = mr;
}

void AMesh::setTextureResource(ATextureResource* t){
	mesh->setTextureResource(t);
}

void AMesh::setMaterialResource(AMaterialResource* m){
	mesh->setMaterialResource(m);
}

void AMesh::setMeshSilhouette(bool s){
	silhouette = s;
}

AMeshResource* AMesh::getMeshResource() const{
	return mesh;
}

ATextureResource* AMesh::getTextureResource() const{
	return mesh->getTextureResource();
}

AMaterialResource* AMesh::getMaterialResource() const{
	return mesh->getMaterialResource();
}