#include "CheckPlayer.h"
#include "Player.h"
#include "Dungeon.h"
#include "Enemy.h"
#include "SMemory.h"
#include <iostream>

CheckPlayer::CheckPlayer(){

}

CheckPlayer::~CheckPlayer(){

}

bool CheckPlayer::execute(int id){
    return check(id);
}

bool CheckPlayer::check(int id){

	//We will come here if we arent detecting the player
	//What we will check is if there's any entity in our memory that has changed in some way
	//from the previous time we saw it

	//If we didnt detect the player we will move to the last position he was and try to see detect him again
	//with a timer. When this timer ends the changed bool inside the memory of the player will change to false
	//and we will then check the others entities

	auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);
    if(npc){
	    SMemory* memory = npc->getMemory();
	    
	    //Let's check if we saw the player with a simple decision tree

	    if(memory->checkEntityType(PLAYER_ENTITY)){
	    	//The ai saw the player --> this is the priority number one for our ai
	    	//We have to check first if the position has been changed
	    	SMemory_Slot* slot = memory->getSlotPerID(Player::Instance()->getID());
	    	if(slot->getChanged()){ //We have to look into it
	    		return true;
	    	}else{
	    		//We already did something about this --> Let's check the other priorities
	    		return false;
	    	}
	    }else{
	    	//Let's check the other priorities
	    	return false;
	    }
	}

    return false;
}

