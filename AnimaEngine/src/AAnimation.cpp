#include "AAnimation.h"
#include <iostream>

AAnimation::AAnimation(int f){

	//anim = new AAnimationResource(f);
    loop = false;
    playing = false;
    pointer = 0;
    animation_clock = 0.0;
    frames = f;
    programID = -1;
    silhouette = true;
    active = true;
}

AAnimation::~AAnimation(){
	anim = nullptr;
}

/*AAnimation::AAnimation(const AAnimation&){

}*/

void AAnimation::loadAnimation(std::string name){
	anim->loadResource(name);
}

void AAnimation::beginDraw(){
	if(active){
		update();
	}
	//load the uniforms
	glm::mat4 m, v, p;
	v = inverse(viewMatrix());
	m = modelMatrix();
	p = projectionMatrix();
	anim->loadUniforms(&m[0][0], &v[0][0], &p[0][0], programID, pointer);

	GLuint loc = glGetUniformLocation(programID, "redraw");
	glUniform1i(loc, silhouette);

	//draw the specific mesh
	anim->draw(pointer);
}

void AAnimation::endDraw(){

}

void AAnimation::play(){
	playing = true;
}

void AAnimation::stop(){
	playing = false;
}

void AAnimation::reset(){
	pointer = 0;
}

void AAnimation::update(){
	
	//update the animation
	if(playing == true){

		//update the animation frame
		if(animation_clock < glfwGetTime() && pointer < frames-1){
			pointer++;
			animation_clock = glfwGetTime() + 0.02;
		}

		//prueba animacion interpolada (no descomentar si no quieres que todo explote)
		/*if(pointer < frames-1){
			animation_clock = glfwGetTime() - animation_clock;
			pointer = animation_clock / 0.02;
		}*/

		//if is an animation on loop and it has reached the end frame then reset
		if(pointer == frames-1 && loop == true){
			reset();
		}

	}
}

/*-----------SETTERS-----------*/
void AAnimation::setFrames(int f){
	frames = f;
}

void AAnimation::setFrame(int f){
	pointer = f;
}

void AAnimation::setLoop(bool l){
	loop = l;
}

void AAnimation::setAnimationSilhouette(bool s){
	silhouette = s;
}

void AAnimation::setProgramId(GLuint pid){
	programID = pid;
}

void AAnimation::setTextureResource(ATextureResource* tr){
	anim->setTextureResource(tr);
}

void AAnimation::setMaterialResource(AMaterialResource* mr){
	anim->setMaterialResource(mr);
}

void AAnimation::setAnimationResource(AAnimationResource* a){
	anim = a;
}

void AAnimation::setAnimationActive(bool a){
	active = a;
}