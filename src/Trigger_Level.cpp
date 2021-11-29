#include "Trigger_Level.h"
#include "TriggerRegion_Rectangle.h"
#include "EventManager.h"
#include "Player.h"
#include "Dungeon.h"

Trigger_Level::Trigger_Level(){
	setInactive();
	setToRemove(false);
	setType(TRIGGER_LEVEL);
	toLevel = -1;
}

Trigger_Level::~Trigger_Level(){
	delete m_regionOfInfluence;
	m_regionOfInfluence = nullptr;
}

void Trigger_Level::Try(){
	auto* pl = Player::Instance();
	auto* e = EventManager::Instance();
	if(toLevel != -1){
		if(m_regionOfInfluence != nullptr){
			if(m_regionOfInfluence->isTouching(Vector2f(pl->getX(), pl->getZ()))){
				Dungeon::Instance()->setLevel(toLevel);
				e->addEvent(CHANGE_LEVEL);
				e->addEvent(ADD_START);
			}
		}
	}
}

void Trigger_Level::updateEntity(){
	Try();
}

void Trigger_Level::setToLevel(int lvl){
	toLevel = lvl;
}