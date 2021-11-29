#include "Trigger_NEnemies.h"
#include "TriggerRegion_Rectangle.h"
#include "EventManager.h"
#include "P_Soldier.h"
#include "Dungeon.h"
#include "Player.h"
#include "AudioManager.h"

Trigger_NEnemies::Trigger_NEnemies(){
	setInactive();
	setToRemove(false);
	setType(TRIGGER_NENEMIES);
	valor = -1;
}

Trigger_NEnemies::~Trigger_NEnemies(){

}

void Trigger_NEnemies::setValor(int v){
	valor = v;
}

void Trigger_NEnemies::Try(){
	bool alldead = true;

	auto* pl = Player::Instance();
	auto* d = Dungeon::Instance();
	auto* e = EventManager::Instance();
	//We check if in the room the player is are no enemies left (their death bool is true)
	int idr = d->wichRoomIAmAt(pl->getX(), pl->getZ());
	std::vector<int> enemies = d->getEnemiesPerRoomID(idr);

	int aux = enemies.size();

	if(aux > 0){
		for(int i = 0; i<aux; i++){
			Enemy* e = d->getEnemyPerID(enemies[i]);
			if(!e->getDeath()){
				alldead = false;
			}
		}
	}

	if(alldead){
		AudioManager::Instance()->shootAudio("Diseño/Juego/PuzzleResuelto",-1,-1);
		e->addEvent(UNLOCK_DOORS);
		e->addEvent(DELETE_ENEMIES);
		switch(valor){
			case 0:
				e->addEvent(CREATE_CHEST_NORMAL_KEY);
			break;
			case 1:
				e->addEvent(CREATE_CHEST_LEVEL_KEY);
			break;
			case 2:
				e->addEvent(CREATE_CHEST_JAIL_KEY);
			break;
			case 3:
				e->addEvent(CREATE_CHEST_BOSS_KEY);
			break;
			case 4:
				e->addEvent(CREATE_CHEST_HAMMER);
			break;
			case 5:
				e->addEvent(CREATE_CHEST_SLINGSHOT);
			break;
			case 6:
				e->addEvent(CREATE_CHEST_FOOD_BERRY);
			break;
			case 7:
				e->addEvent(CREATE_CHEST_FOOD_MEAT);
			break;
			case 8:
				e->addEvent(CREATE_CHEST_FOOD_FISH);
			break;
			case 9:
				e->addEvent(CREATE_CHEST_FOOD_ICE);
			break;
			case 10:
				e->addEvent(CREATE_QUEEN);
			break;
			default:
				e->addEvent(CREATE_CHEST_NORMAL_KEY);
			break;
		}
		setActive();
	}
}

void Trigger_NEnemies::updateEntity(){
	if(isActive() && !isToBeRemoved()){
		setToRemove(true);
	}
	else{
		Try();
	}
}