#include "ALight.h"

ALight::ALight(){
	intensity = glm::vec3(1,1,1);
}

ALight::~ALight(){

}

void ALight::setIntensity(glm::vec3 i){
	intensity = i;
}

glm::vec3 ALight::getIntensity() const{
	return intensity;
}

void ALight::beginDraw(){
	//vacio
}

void ALight::endDraw(){
	//vacio
}