#include "Trigger_Enemies.h"
#include "TriggerRegion_Rectangle.h"
#include "EventManager.h"
#include "P_Soldier.h"
#include "Dungeon.h"
#include "Player.h"

Trigger_Enemies::Trigger_Enemies(){
	setInactive();
	setToRemove(false);
	setType(TRIGGER_ENEMIES);
}

Trigger_Enemies::~Trigger_Enemies(){
	delete m_regionOfInfluence;
	m_regionOfInfluence = nullptr;
}

void Trigger_Enemies::Try(){
	auto* pl = Player::Instance();
	auto* e = EventManager::Instance();
	if(m_regionOfInfluence != nullptr){
		if(m_regionOfInfluence->isTouching(Vector2f(pl->getX(), pl->getZ()))){
			e->addEvent(LOCK_DOORS);
			e->addEvent(CREATE_ENEMIES);
			setActive();
		}
	}
}

void Trigger_Enemies::updateEntity(){
	if(isActive() && !isToBeRemoved()){
		setToRemove(true);
	}
	else{
		Try();
	}
}