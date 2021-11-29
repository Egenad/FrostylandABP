#include "ACamera.h"

ACamera::ACamera(float u, float d, float l, float r, float n, float f){
	isPerspective = true;
	up=u;
	down=d;
	left=l;
	right=r;
	near=n;
	far=f;

	//detail::tmat4x4< T > 	frustum (T const &left, T const &right, T const &bottom, T const &top, T const &nearVal, T const &farVal)
	glm::mat4& p = projectionMatrix();
	p = glm::frustum(left, right, down, up, near, far);
}

ACamera::ACamera(float fov, float aspect, float n, float f){
	isPerspective = true;
	far=f;
	near=n;

	//detail::tmat4x4< T > 	perspective (T const &fovy, T const &aspect, T const &zNear, T const &zFar)
	glm::mat4& p = projectionMatrix();
	p = glm::perspective(fov, aspect, near, far);
}

ACamera::~ACamera(){
}

void ACamera::setPerspective(float fov, float aspect, float n, float f){
	//sets the camera with fov and aspect parameters
	isPerspective = true;
	far=f;
	near=n;

	//detail::tmat4x4< T > 	perspective (T const &fovy, T const &aspect, T const &zNear, T const &zFar)
	glm::mat4& p = projectionMatrix();
	p = glm::perspective(fov, aspect, near, far);
}

void ACamera::setPerspective(float u, float d, float l, float r, float f, float n){
	//																			 __
	//sets the camera using the frustum shape parameters (piramidal w/o the top /__\)
	isPerspective = true;
	up=u;
	down=d;
	left=l;
	right=r;
	far=f;
	near=n;

	//detail::tmat4x4< T > 	frustum (T const &left, T const &right, T const &bottom, T const &top, T const &nearVal, T const &farVal)
	glm::mat4& p = projectionMatrix();
	p = glm::frustum(left, right, down, up, near, far);
}

void ACamera::setParalell(float u, float d, float l, float r, float f, float n){
	isPerspective = false;
	up=u;
	down=d;
	left=l;
	right=r;
	far=f;
	near=n;

	//detail::tmat4x4< T > 	ortho (T const &left, T const &right, T const &bottom, T const &top, T const &zNear, T const &zFar)
	glm::mat4& p = projectionMatrix();
	p = glm::ortho(left, right, down, up, near, far);
	
}

void ACamera::beginDraw(){
	//vacio
}

void ACamera::endDraw(){
	//vacio
}