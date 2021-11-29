#include "SMemory.h"

SMemory::SMemory(){

}

SMemory::~SMemory(){
	for(long unsigned int i=0; i<p_entities.size(); i++){
		delete p_entities[i];
		p_entities[i] = nullptr;
	}
}

void SMemory::reportEntities(EntityFlag f){
	
	bool exists = false;

	//Check if it exists already

	for(long unsigned int i=0; i<p_entities.size(); i++){
		EntityFlag flag = p_entities[i]->getEFlag();
		if(flag.id == f.id){ //exists --> check if everything is the same as last time the ai detected it
			if(flag.p_x != f.p_x || flag.p_y != f.p_y  || flag.p_z != f.p_z ){
				p_entities[i]->setParameters(f.p_x, f.p_y, f.p_z);
			}
			exists = true;
		}
	}

	if(!exists){
		int pr = 0;
		switch(f.type){
			case NORMAL_DOOR_ENTITY:
				pr = 2;
			break;
			case PLAYER_ENTITY:
				pr = 3;
			break;
			default:
			  	pr = 1;
			break;
		}

		SMemory_Slot* slot = new SMemory_Slot(f.id, f.type,Vector3f(f.p_x, f.p_y, f.p_z), pr);
		p_entities.push_back(slot);
	}
}

std::vector<SMemory_Slot*>& SMemory::GetAllPerceivedEntities(){
	return p_entities;
}

SMemory_Slot* SMemory::getSlotPerID(int idr){
	for(long unsigned int i=0; i<p_entities.size(); i++){
		EntityFlag flag = p_entities[i]->getEFlag();
		if(flag.id == idr){
			return p_entities[i];
		}
	}
	return nullptr;
}

bool SMemory::checkEntityType(EntityType type){
	//In this method we check if the ai saw any entity of the type we passed through parameter

	for(long unsigned int i=0; i<p_entities.size(); i++){
		EntityFlag flag = p_entities[i]->getEFlag();
		if(flag.type == type){
			return true;
		}
	}

	return false;
}