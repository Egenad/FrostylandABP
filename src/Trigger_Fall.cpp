#include "Trigger_Fall.h"
#include "TriggerRegion_Rectangle.h"
#include "EventManager.h"
#include "Player.h"

Trigger_Fall::Trigger_Fall(){
	setInactive();
	setToRemove(false);
	setType(TRIGGER_FALL);
}

Trigger_Fall::~Trigger_Fall(){
	delete m_regionOfInfluence;
	m_regionOfInfluence = nullptr;
}

void Trigger_Fall::Try(){
	auto* pl = Player::Instance();
	auto* e = EventManager::Instance();

	if(m_regionOfInfluence != nullptr){
		if(m_regionOfInfluence->isTouching(Vector2f(pl->getX(), pl->getZ()))){
			if(pl->getY()<-30){
				e->addEvent(PLAYER_FALLS);
			}
			//setActive(); supuestamente esto dice angel que chao
		}
	}
}

void Trigger_Fall::updateEntity(){
	if(isActive() && !isToBeRemoved()){
		setToRemove(true);
	}
	else{
		Try();
	}
}