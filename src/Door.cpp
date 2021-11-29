#include "Door.h"
#include "Dungeon.h"
#include "EventManager.h"
#include "Player.h"
#include "AudioManager.h"

Door::Door(float x, float z, bool b, bool cl, EntityType type){
	ps_x = x;
	ps_z = z;
	ps_y = 0.f;
	w_1 = nullptr;
	w_2 = nullptr;
	if(b){//puerta frontal
		w_1 = new Vector3f(ps_x,9.9f,ps_z+80);
		w_2 = new Vector3f(ps_x,9.9f,ps_z-80);
		if(getType() != NORMAL3_DOOR_ENTITY){
			PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,35,35,9,0,rigidBody,state,bodyShape);
		}else{
			PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z + 50,35,35,9,0,rigidBody,state,bodyShape);
		}
	}else{//puerta lateral
		w_1 = new Vector3f(ps_x-80,9.9f,ps_z);
		w_2 = new Vector3f(ps_x+80,9.9f,ps_z);
		PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,9,35,35,0,rigidBody,state,bodyShape);
	}
	down = b;
	closed = cl;
	locked = false;
	inamovible = false;
	setType(type);
	setRadiusOfInteract(30);
}

Door::~Door(){
	if(rigidBody){
		delete rigidBody;
		rigidBody = nullptr;
	}
	if(state){
		delete state;
		state = nullptr;
	}
	if(bodyShape){
		delete bodyShape;
		bodyShape = nullptr;
	}
	delete w_1;
	delete w_2;
	m3D::Motor3D::Instance()->clearNode(object);
}

void Door::updateEntity(){
	auto* m = m3D::Motor3D::Instance();
	auto* d = Dungeon::Instance();
	float aux = d->getLevelMediaY() + 2;
	if(!closed && ps_y < 100){
		if(getType() != JAIL_DOOR_ENTITY){
			ps_y = ps_y + (160 * m->getDeltaTime());
		}else{
			ps_y = ps_y + 100;
		}
		if(getType() != NORMAL3_DOOR_ENTITY){
			PhysicsEngine::Instance()->reposition(ps_x, ps_y, ps_z, rigidBody, state);
		}else{
			PhysicsEngine::Instance()->reposition(ps_x, ps_y, ps_z+50, rigidBody, state);
		}
		m->setPositionEntity(object,ps_x,ps_y,ps_z);
	}else if(closed && ps_y > aux && !inamovible){
		ps_y = ps_y - (160 * m->getDeltaTime());
		if(getType() != NORMAL3_DOOR_ENTITY){
			PhysicsEngine::Instance()->reposition(ps_x, ps_y, ps_z, rigidBody, state);
		}else{
			PhysicsEngine::Instance()->reposition(ps_x, ps_y, ps_z+50, rigidBody, state);
		}
		m->setPositionEntity(object,ps_x,ps_y,ps_z);
	}
}

void Door::drawDoor(){
	m3D::Motor3D::Instance()->drawDoor(object, ps_x, ps_y, ps_z, down, getType());
}

bool Door::interactPlayer(float ent_x, float ent_z){
	auto* pl = Player::Instance();
	auto* e = EventManager::Instance();

	if(closed && !locked){  
    	if(getType() == BOSS_DOOR_ENTITY && pl->getBossKey()){
    		//Open Boss door
    		this->playAudio(1);
    		closed = false;
	    	pl->setBossKey(false);
	    	e->addEvent(BOSS_DOOR_ANIMATION_START);
	        return true;
    	}else if(getType() == LEVEL_DOOR_ENTITY && pl->getLevelKey()){
	    	//Open lvl door
    		this->playAudio(2);
    		closed = false;
	    	pl->setLevelKey(false);
	        return true;
	    }else if(getType() == JAIL_DOOR_ENTITY && pl->getJailKey()){
	    	//Open jail door
    		this->playAudio(4);
    		closed = false;
	    	pl->setJailKey(false);
	        return true;
	    }else if((getType() == NORMAL_DOOR_ENTITY || getType() == NORMAL2_DOOR_ENTITY) && pl->getKeys() > 0){
	    	//Open normal door
    		this->playAudio(3);
    		closed = false;
	    	pl->restKey();
	        return true;
	    }else{
    		this->playAudio(0);
	    }
	}

    return false;
}

//Setters

void Door::repositionRigidBody(){
	PhysicsEngine::Instance()->reposition(ps_x, ps_y, ps_z, rigidBody, state);
}

void Door::setClosed(bool b){
	if(b){
    	this->playAudio(6);
	}else{
		this->playAudio(5);
	}
	closed = b;
}

void Door::setInamovible(bool b){
	inamovible = b;
}

void Door::setIDs(int r1, int r2){
	this->r1 = r1;
	this->r2 = r2;
}

void Door::setIndex(int r1, int r2){
	index1 = r1;
	index2 = r2;
}

void Door::setLocked(bool l){
	locked = l;
}

//Getters
Vector3f Door::getWaypoints(int val){
	if(w_1 != nullptr && w_2 != nullptr){
		if(val == 1){
			return *w_1;
		}else{
			return *w_2;
		}
	}
	return Vector3f(-1,-1,-1);
}

bool Door::getClosed(){
	return closed;
}

bool Door::getLocked(){
	return locked;
}

bool Door::getInamovible(){
	return inamovible;
}

int Door::getID1(){
	return r1;
}

int Door::getID2(){
	return r2;
}

int Door::getINDEX1(){
	return index1;
}

int Door::getINDEX2(){
	return index2;
}

bool Door::getDown(){
	return down;
}

Vector3f Door::getPosition(){
	Vector3f pos = Vector3f(ps_x,ps_y,ps_z);
	return pos;
}

void Door::playAudio(int a){
    auto* am = AudioManager::Instance();
    float r = ((double) rand() / (RAND_MAX));

    if(r == 0.3){
        r = 0.4;
    }else if(r == 0.5){
        r = 0.4;
    }else if(r == 0.6){
        r = 0.55;
    }
  	
    if(a == 0){
    	//Locked Door
    	am->shoot3DAudio("HardFX/Puerta/Bloqueada",0,r, Vector3f(ps_x, ps_y, ps_z));	

    }else if(a >= 1 && a <= 4){
    	//Unlock door
    	am->shoot3DAudio("HardFX/Puerta/Desbloquear",0,r, Vector3f(ps_x, ps_y, ps_z));
    	if(a >= 1 && a <= 3){
    		//Wood door
    		am->shoot3DAudio("HardFX/Puerta/Abrir",0,r, Vector3f(ps_x, ps_y, ps_z));	

    		// 1 -> Boss door // 2 -> lvl door // 3 -> normal door // 4 -> jail door
    		if(a == 1){
    			//Boss Door
    			am->shoot3DAudio("Diseño/Puzzle/Resuelto",-1,0, Vector3f(ps_x, ps_y, ps_z));
    		}
    	}
    }else if(a == 5){
    	//open door
    	am->shoot3DAudio("HardFX/Puerta/Abrir",0,r, Vector3f(ps_x, ps_y, ps_z));	
    }else if(a == 6){
    	//Close door
    	am->shoot3DAudio("HardFX/Puerta/Cerrar",0,r, Vector3f(ps_x, ps_y, ps_z));	
    }

    am = nullptr;

}