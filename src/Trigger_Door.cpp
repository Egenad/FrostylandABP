#include "Trigger_Door.h"
#include "TriggerRegion_Rectangle.h"
#include "EventManager.h"
#include "Player.h"

Trigger_Door::Trigger_Door(){
	setInactive();
	setToRemove(false);
	setType(TRIGGER_DOOR);
}

Trigger_Door::~Trigger_Door(){

}

void Trigger_Door::Try(){
	auto* pl = Player::Instance();
	auto* e = EventManager::Instance();

	if(m_regionOfInfluence != nullptr){
		if(m_regionOfInfluence->isTouching(Vector2f(pl->getX(), pl->getZ()))){
			e->addEvent(LOCK_DOORS);
			setActive();
		}
	}
}

void Trigger_Door::updateEntity(){
	if(isActive() && !isToBeRemoved()){
		setToRemove(true);
	}
	else{
		Try();
	}
}