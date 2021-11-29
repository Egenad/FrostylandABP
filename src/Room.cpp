#include "Room.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include "Trigger_Enemies.h"
#include "EventManager.h"
#include "Trigger_NEnemies.h"
#include "DragBox.h"
#include "Chest.h"
#include "Decoration.h"

Room::Room(float x, float z, float w, float h){
	//Assing the center point of the room
	width = w;
	height = h;
	center_x = x + w/2;
	center_z = z + h/2;
	//We init now the level of alert that the room has
	alert = 0;
}

Room::~Room(){

	for(long unsigned int i=0; i<triggers.size(); i++){
		delete triggers[i];
		triggers[i] = nullptr;
	}

	triggers.resize(0);

	for(long unsigned int i=0; i<interactibles.size(); i++){
		delete interactibles[i];
		interactibles[i] = nullptr;
	}

	interactibles.resize(0);

	for(long unsigned int i=0; i<platforms.size(); i++){
		delete platforms[i];
		platforms[i] = nullptr;
	}

	platforms.resize(0);

	for(long unsigned int i=0; i<puzzles.size(); i++){
		delete puzzles[i];
		puzzles[i] = nullptr;
	}

	puzzles.resize(0);

	for(long unsigned int i=0; i<decorations.size(); i++){
        if(decorations[i] != nullptr){            
            decorations[i]->playAudio(0);
            delete decorations[i];
            decorations[i] = nullptr;
        }
    }
    decorations.resize(0);
}

void Room::updateEntity(){

	if(triggers.size() > 0 && triggers[0] != nullptr){
		if(!triggers[0]->isToBeRemoved()){
			triggers[0]->updateEntity();
		}else{
			delete triggers[0];
			triggers[0] = nullptr;
			triggers.erase(triggers.begin());
		}	
	}

	bool done = false;
	for(long unsigned int i=0; i<triggers.size() && !done; i++){
		if(triggers[i]->getType() == TRIGGER_CAMERA){
			triggers[i]->updateEntity();
			done = true;
		}
	}

	for(long unsigned int i=0; i<interactibles.size(); i++){
		if(interactibles[i]->getType() == PRESSED_INTERACTIBLE){
			interactibles[i]->updateEntity();
		}
	}

	for(long unsigned int i=0; i<platforms.size(); i++){
		platforms[i]->updateEntity();
	}

	int aux = getID();

	for(long unsigned int i=0; i<puzzles.size(); i++){
		puzzles[i]->checkConditions(aux);
	}

    for(long unsigned int i=0; i<decorations.size(); i++){
        decorations[i]->updateEntity();
    }
}

void Room::drawInteractuables(){

	auto* m = m3D::Motor3D::Instance();

	for(long unsigned int i=0; i<interactibles.size(); i++){
		auto* in = interactibles[i];
		if(in->getType() == CHEST_INTERACTIBLE){
			m->drawChest(in->getNode(), in->getX(), in->getY(), in->getZ());
			Chest* c = dynamic_cast<Chest*> (in);
			if(c->getOpen())
				m->setOpenedChest(in->getNode());
		}else{
			m->drawInteractible(in->getNode(), in->getX(), in->getY(), in->getZ(), in->getType());
		}
		
		in = nullptr;
	}

	for(long unsigned int i=0; i<platforms.size(); i++){
		auto* in = platforms[i];
		m->drawPlatform(in->getNode(), in->getX(), in->getY(), in->getZ());
		in = nullptr;
	}

	for(long unsigned int i=0; i<decorations.size(); i++){
        decorations[i]->drawDecoration();
        decorations[i]->playAudio(1);
    }

	m = nullptr;
}

//Setters

void Room::setTorchsActive(bool f){
	for(long unsigned int i=0; i<decorations.size(); i++){
		if(decorations[i]->getType() == DECORATION_TORCH){
			m3D::Motor3D::Instance()->setAnimationActive(decorations[i]->getNode(), f);
		}
	}
}

void Room::addDecoration(Decoration* d){
    decorations.push_back(d);
}

void Room::addAlert(int a){
	alert += a;
}

void Room::quitAlert(int a){
	alert -= a;
	if(alert<0){
		alert = 0;
	}
}

void Room::addinteractible(Interactible* c){
	interactibles.push_back(c);
}

void Room::addPlatform(Platform* p){
	platforms.push_back(p);
}

void Room::addPuzzle(Puzzle* p){
	puzzles.push_back(p);
}

//Getters
int Room::getAlert(){
	return alert;
}

float Room::getCenterX(){
	return center_x;
}

float Room::getCenterZ(){
	return center_z;
}

float Room::getHeight(){
	return height;
}

float Room::getWidth(){
	return width;
}

float Room::getMaxX(){
	return center_x + (width/2);
}

float Room::getMinX(){
	return center_x - (width/2);
}

float Room::getMaxZ(){
	return center_z + (height/2);
}

float Room::getMinZ(){
	return center_z - (height/2);
}

bool Room::interactibleHere(){
	if(interactibles.size() > 0){
		return true;
	}
	return false;
}

void Room::addTrigger(Trigger* t){
	triggers.push_back(t);
}

void Room::eraseInteractible(int idi){
	for(long unsigned int i=0; i<interactibles.size(); i++){
		if(interactibles[i]->getID() == idi){
			interactibles.erase(interactibles.begin() + i);
		}
	}
}

void Room::eraseTrigger(){
	if(triggers.size() > 0 && triggers[0] != nullptr){
		delete triggers[0];
		triggers[0] = nullptr;
		triggers.erase(triggers.begin());
	}
}

bool Room::interactueWithObjectInRange(float x, float z){
	bool interacted = false; //We dont want to interact with more than one object at the same time
	for(long unsigned int i=0; i<interactibles.size() && !interacted; i++){
		//Check if there's an object that is in range of the entity that tries to use it
		EntityType aux = interactibles[i]->getType();
		if( aux == BUTTON_INTERACTIBLE || aux == CHEST_INTERACTIBLE || aux == DRAGGABLE_INTERACTIBLE){
			if(interactibles[i]->interactEntity(x,z)){
				interacted = true;
				interactibles[i]->updateEntity();
			}
		}
	}
	return interacted;
}

Vector3f Room::getClosestDecoration(float x,float z){

	Vector3f pos(0.f,0.f,0.f);

	for(long unsigned int i=0; i<decorations.size(); i++){
        float x2 = decorations[i]->getX();
        float z2 = decorations[i]->getZ();
        float distx = x - x2;
        float distz = z - z2;
        distx = distx * distx;
        distz = distz * distz;
        float dist = sqrt(distx + distz);
        if(dist <= 150){
            if(pos.getX() == 0 && pos.getZ() == 0){
                pos.setX(x2);
                pos.setZ(z2);
            }else if(pos.getX() > x2 && pos.getZ() > z2){
                pos.setX(x2);
                pos.setZ(z2);
            }
        }
    }

    return pos;
}

bool Room::interactibleInRange(float x, float z){

	bool interactible = false; 
	for(long unsigned int i=0; i<interactibles.size() && !interactible; i++){
		//Check if there's an object that is in range 
		EntityType aux = interactibles[i]->getType();
		if( aux == BUTTON_INTERACTIBLE || aux == CHEST_INTERACTIBLE || aux == DRAGGABLE_INTERACTIBLE){
			if(interactibles[i]->interactEntity(x,z)){
				Chest* chest = dynamic_cast<Chest*> (interactibles[i]);
            	auto* m = m3D::Motor3D::Instance();
				
				if(chest){
                	if(chest->getOpen() == false){
                		//cofre
                        m->drawInfoControl(0);
						interactible = true;
                	}
				}else{
                	//DragBox
                    m->drawInfoControl(1);
					interactible = true;
				}
				m=nullptr;
				chest=nullptr;
			}
		}
	}
	return interactible;
}

bool Room::checkCollisionWithBox(Interactible* plate){
	for(long unsigned int i=0; i<interactibles.size(); i++){
		if(interactibles[i]->getType() == DRAGGABLE_INTERACTIBLE){
			if(plate->interactEntity(interactibles[i]->getX(), interactibles[i]->getZ())){
				return true;
			}
		}
	}
	return false;
}

int Room::IDwichCollides(Interactible* plate){
	for(long unsigned int i=0; i<interactibles.size(); i++){
		if(interactibles[i]->getType() == DRAGGABLE_INTERACTIBLE){
			if(plate->interactEntity(interactibles[i]->getX(), interactibles[i]->getZ())){
				DragBox* box = dynamic_cast<DragBox*> (interactibles[i]);
				return box->getBoxId();
			}
		}
	}
	return -1;
}

std::vector<Interactible*>& Room::getInteractibles(){
	return interactibles;
}

std::vector<Puzzle*>& Room::getPuzzles(){
	return puzzles;
}

Interactible* Room::getInteractiblePerID(int id){

	Interactible* inter = nullptr;

	for(long unsigned int i=0; i<interactibles.size(); i++){
		if(interactibles[i]->getID() == id){
			inter = interactibles[i];
			break;
		}
	}

	return inter;
}

std::vector<Trigger*>& Room::getTriggers(){
	return triggers;
}

Decoration* Room::getTable(){
	for(long unsigned int i = 0; i < decorations.size(); i++){
		if(decorations[i]->getType() == DECORATION_TABLE){
			return decorations[i];
		}
	}
	return nullptr;
}

Decoration* Room::getStatue1(){
	for(long unsigned int i = 0; i < decorations.size(); i++){
		if(decorations[i]->getType() == DECORATION_STATUE1){
			return decorations[i];
		}
	}
	return nullptr;
}

Decoration* Room::getStatue2(){
	for(long unsigned int i = 0; i < decorations.size(); i++){
		if(decorations[i]->getType() == DECORATION_STATUE2){
			return decorations[i];
		}
	}
	return nullptr;
}
