#include "Trigger_Camera.h"
#include "TriggerRegion_Rectangle.h"
#include "EventManager.h"
#include "Player.h"

Trigger_Camera::Trigger_Camera(){
	setInactive();
	setToRemove(false);
	setType(TRIGGER_CAMERA);
	touched = false;
}

Trigger_Camera::~Trigger_Camera(){
	delete m_regionOfInfluence;
	m_regionOfInfluence = nullptr;
}

void Trigger_Camera::Try(){
	auto* pl = Player::Instance();
	auto* e = EventManager::Instance();

	if(m_regionOfInfluence != nullptr){
		if(m_regionOfInfluence->isTouching(Vector2f(pl->getX(), pl->getZ()))){
			e->addEvent(CAMERA_ZOOM_IN);
			touched = true;
		}else if(touched){
			e->addEvent(CAMERA_ZOOM_OUT);
			touched = false;
		}
	}
}

void Trigger_Camera::updateEntity(){
	Try();
}