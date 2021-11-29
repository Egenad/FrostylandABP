#include "MoveTo.h"
#include "Dungeon.h"
#include "Enemy.h"
#include "SMemory.h"
#include "Motor3D.h"
#include "Player.h"

MoveTo::MoveTo(){
	look = false;
	vect_change = 0;
	changed = false;
}

MoveTo::~MoveTo(){

}

bool MoveTo::execute(int id){
    return movetopoint(id);
}

bool MoveTo::movetopoint(int id){
	auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);
    if(npc){
    	SMemory* memory = npc->getMemory();
    	if(!look){ //We want to, first, move to the last position we saw the player at
		    if(memory->checkEntityType(PLAYER_ENTITY)){
		    	SMemory_Slot* slot = memory->getSlotPerID(Player::Instance()->getID());
		    	EntityFlag flag = slot->getEFlag();
		    	if(d->wichRoomIAmAt(flag.p_x, flag.p_z) != d->wichRoomIAmAt(npc->getX(), npc->getZ())){
		    		return false;
		    	}
		    	npc->seek(Vector3f(flag.p_x, flag.p_y, flag.p_z));
		    	if((int)abs(npc->getX() - flag.p_x) < 1 && (int)abs(npc->getZ() - flag.p_z) < 1 && !look){
		    		look = true;
		    		npc->setVelocityX(0.f);
       				npc->setVelocityZ(0.f);
       				npc->setDeactivated(true);		
		    	}
			}
		}else{ //When we are at the point we start looking around us
			switch(vect_change){
				case 1:
					if(check()){
						npc->setVisionVectorValues(0,0,1);
					}
				break;
				case 2:
					if(check()){
						npc->setVisionVectorValues(0,0,-1);
					}
				break;
				case 3:
					if(check()){
						npc->setVisionVectorValues(1,0,0);
						look = false;
						vect_change = 0;
						SMemory_Slot* slot = memory->getSlotPerID(Player::Instance()->getID());
						slot->setChanged(false);
						slot = nullptr;
						npc->setDeactivated(true);
					}
				break;
				default:
					if(check()){
						npc->setVisionVectorValues(-1,0,0);
					}
				break;
			}
		}
		memory = nullptr;
	}

	return true;
}

bool MoveTo::check(){
	auto* m = m3D::Motor3D::Instance();
	if(!changed){
		then = m->getNow() + 1;
		changed = true;
		return false;
	}else if(m->getNow() > then){
		changed = false;
		vect_change += 1;
		return true;
	}
	return false;
}