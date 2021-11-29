#include "SMemory_Slot.h"
#include <iostream>
SMemory_Slot::SMemory_Slot(int idr, EntityType t, Vector3f f, int pr){
	entity.id = idr;
	entity.p_x = f.getX();
	entity.p_y = f.getY();
	entity.p_z = f.getZ();
	entity.type = t;
	priority = pr;
	changed = false;
	if(pr == 3){
		changed = true;
	}
}

SMemory_Slot::~SMemory_Slot(){

}


/////////////////////
//Setters
/////////////////////

void SMemory_Slot::setParameters(float x, float y, float z){
	entity.p_x = x;
	entity.p_y = y;
	entity.p_z = z;
	changed = true;
}

void SMemory_Slot::setChanged(bool f){
	changed = f;
}

/////////////////////
//Getters
/////////////////////

int SMemory_Slot::getPriority(){
	return priority;
}

EntityFlag SMemory_Slot::getEFlag(){
	return entity;
}

bool SMemory_Slot::getChanged(){
	return changed;
}