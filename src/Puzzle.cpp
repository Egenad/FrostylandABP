#include "Puzzle.h"
#include "Dungeon.h"
#include "Plate.h"

Puzzle::Puzzle(){
	resolved = false;
	conditions.reserve(2);
}

Puzzle::~Puzzle(){
	conditions.resize(0);
}

bool Puzzle::getResolved() const{
	return resolved;
}

void Puzzle::setResolved(bool r){
	resolved = r;
}

void Puzzle::checkConditions(int idr){
	auto* d = Dungeon::Instance();
	Room* r = d->getRoomPerID(idr);
	bool done = true;
	for(long unsigned int i=0; i<conditions.size(); i++){
		switch(conditions[i].type){
			case PRESSED_INTERACTIBLE:{
				Interactible* inter = r->getInteractiblePerID(conditions[i].id);
				if(inter){
					if(inter->getY() != conditions[i].p_y){
						done = false;
					}
				}else{
					done = false;
				}
				break;
			}
			case DRAGGABLE_INTERACTIBLE:{
				DragBox* db = d->getDragBoxPerID(conditions[i].id);
				if(db){
					int aux = db->getX();
					int aux2 = conditions[i].p_x;
					int aux3 = db->getZ();
					int aux4 = conditions[i].p_z;
					if(!(aux <= aux2 + 20 && aux >= aux2 - 20) || !(aux3 <= aux4 + 20 && aux3 >= aux4 - 20)){
						done = false;
					}
				}else{
					done = false;
				}
				break;
			}
			default:
			break;
		}
	}

	if(done){
		resolved = true;
	}else{
		resolved = false;
	}

	d = nullptr;
	r = nullptr;
}

void Puzzle::addCondition(EntityFlag ef){
	conditions.push_back(ef);
}

std::vector<EntityFlag> Puzzle::getConditions(){
	return conditions;
}

void Puzzle::playAudio(int a){
	
}