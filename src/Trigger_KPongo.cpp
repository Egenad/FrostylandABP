#include "Trigger_KPongo.h"
#include "TriggerRegion_Rectangle.h"
#include "Dungeon.h"
#include "Player.h"

Trigger_KPongo::Trigger_KPongo(){
	setInactive();
	setToRemove(false);
	setType(TRIGGER_KPONGO);
}

Trigger_KPongo::~Trigger_KPongo(){
	delete m_regionOfInfluence;
	m_regionOfInfluence = nullptr;
}

void Trigger_KPongo::Try(){
	auto* pl = Player::Instance();

	if(m_regionOfInfluence != nullptr){
		if(m_regionOfInfluence->isTouching(Vector2f(pl->getX(), pl->getZ()))){
			Dungeon::Instance()->setKPongoBattle();
			setActive();
		}
	}
}

void Trigger_KPongo::updateEntity(){
	if(isActive() && !isToBeRemoved()){
		setToRemove(true);
	}
	else{
		Try();
	}
}