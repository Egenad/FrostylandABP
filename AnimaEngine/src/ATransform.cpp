#define GLM_ENABLE_EXPERIMENTAL //glm transform is actually on development
#include "ATransform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glut.h>
#include <iostream>

ATransform::ATransform(){
	matrix = glm::mat4(1.0f);
}

ATransform::~ATransform(){

}

void ATransform::identity(){
	matrix = glm::mat4(1.0f);
}

void ATransform::load(glm::mat4 m){
	matrix = m;
}

void ATransform::traspose(){
	matrix = glm::transpose(matrix); 	
}

void ATransform::traslate(float x, float y, float z){
		//traslation matrix = identity matrix with the coordinates on the las column													
		//		|1 0 0 x|
		// MT = |0 1 0 y|
		//		|0 0 1 z|		
		//		|0 0 0 1|
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
}

void ATransform::rotate(float ux, float uy, float uz, float angle){
	//ux, uy y uz are the components of the vector that represents the rotation axis
	glm::vec3 axis(ux,uy,uz);
	matrix = glm::rotate(matrix,angle,axis);
}

void ATransform::scale(float x, float y, float z){												
		//		|s 0 0 0|
		// MS = |0 s 0 0|
		//		|0 0 s 0|		
		//		|0 0 0 1|

	matrix = glm::scale(matrix, glm::vec3(x,y,z));
}

glm::mat4 ATransform::getMatrix(){
	return matrix;
}

void ATransform::beginDraw(){
	std::stack<glm::mat4>& mstack = matrixStack();
	glm::mat4& model = modelMatrix();
	//push the actual model matrix to save a copy of it
    mstack.push(model);
	//multiply the model matrix with the matrix of this transform and save it as the actual model matrix
    model=matrix*model;
}

void ATransform::endDraw(){
	glm::mat4& model = modelMatrix();
	std::stack<glm::mat4>& mstack = matrixStack();
	//save the top of the stack as the actual model matrix
	model=mstack.top();
	mstack.pop();
}