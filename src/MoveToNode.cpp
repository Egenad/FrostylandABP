#include "MoveToNode.h"
#include "Dungeon.h"
#include <iostream>
#include "Fox.h"
#include "P_Soldier.h"
#include "Player.h"

MoveToNode::MoveToNode(){
	toDoorW1 = false;
	toDoorW2 = false;
	index = 0;
	toclose = false;
	tolock = false;
	doorindex = 0;
	inverse = false;
	timer = 0;
}

MoveToNode::~MoveToNode(){}

bool MoveToNode::execute(int id){
	return move(id); 
}

bool MoveToNode::move(int id){
	auto* d = Dungeon::Instance();
	Enemy* npc = d->getEnemyPerID(id);
	float aux2 = d->wichRoomIAmAt(npc->getX(), npc->getZ());
	auto* m = m3D::Motor3D::Instance();
   	
	Fox* fox = dynamic_cast<Fox*> (npc);

	PathfindingList* list = fox->getPathList();

    if(fox && list != nullptr){

    	std::vector<NodeRecord> n = list->getNodeList();

    	//If the list only has 1 node it means we are already there
    	if(timer < m->getNow()){
	    	if(wActual.getX() == -1 && wActual.getY() == -1 && n.size() > 1){
	    		if(!inverse){
	    			npc->setMaxVel(100);
	    			index = 0;
	    			wActual = n[index].node;
	    			wNext = n[index+1].node;
	    		}else{
	    			npc->setMaxVel(50);
	    			index = n.size()-1;
	    			wActual = n[index].node;
	    			wNext = n[index-1].node;
	    		}
	    		toDoorW1 = true;
	    	}else{
	    		//We have to search for the door we have to move to.
	    		//How? -> we have the centers of 2 rooms. With those, we can get both ID and from that get the Door
	    		//that is allocated in the middle of both.

	    		int r1 = d->getRoomIDPerCenterInt(wActual);
	    		int r2 = d->getRoomIDPerCenterInt(wNext);

	    		Door* door = d->getDoorPerRoomsID(r1, r2);

	    		if(door){
		    		if(toDoorW1){//Move to the first node of the door and check if the door is closed/locked
		    			//How do we know wich of the 2 nodes of the door we have to move to? -> looking wich one is in our actual
		    			//room.
		    			Vector3f w = door->getWaypoints(1);
		    			float aux = d->wichRoomIAmAt(w.getX(), w.getZ());
		    			if(aux == aux2){ //Thats the node we have to move on
		    				doorindex = 1;
		    			}else{ //It's the other one
		    				doorindex = 2;
		    				w = door->getWaypoints(2);	
		    			}
		    			seek(door, npc, w, 1);
		    			return true;
		    		}else if(toDoorW2){//Move to the second node of the door and if the door was closed/locked, close/lock it.		
		    			Vector3f w;
		    			if(doorindex == 1){
		    				w = door->getWaypoints(2);
		    			}else{
		    				w = door->getWaypoints(1);
		    			}
		    			seek(door, npc, w, 2);
		    			return true;
		    		}else{ //Move to the center of the next room
		    			Vector3f w(wNext.getX(), 0, wNext.getY());
		    			npc->seek(w);
		    			if((int)abs(npc->getX() - w.getX()) < 1 && (int)abs(npc->getZ() - w.getZ()) < 1){
							npc->setVelocityX(0.f);
							npc->setVelocityZ(0.f);
							//npc->setDeactivated(true);
							if(!inverse){
								index++;
								if(index + 1 < n.size()){ //Let's tell the npc that he has to go to the next room
									wActual = n[index].node;
						    		wNext = n[index+1].node;
						    		toDoorW1 = true;
								}else if(n[n.size()-1].node.getX() == wNext.getX() && n[n.size()-1].node.getY() == wNext.getY()){ //Let's check if we are already at the end
									//Here we advise the penguins that are in the room to make them follow us
									std::vector<Enemy*>& en = d->getEnemies();
									for(long unsigned int i=0; i<en.size(); i++){
										float e_x = en[i]->getX();
										float e_z = en[i]->getZ();
										float er = d->wichRoomIAmAt(e_x, e_z);
										if(er == aux2 && en[i]->getType() == P_SOLDIER_ENTITY){
											P_Soldier* psoldier = dynamic_cast<P_Soldier*> (en[i]);
											psoldier->setFollow(true);
											timer = m->getNow() + 3;
											psoldier->setFollowUp(fox->getID());
										}
									}
									inverse = true;
									wActual = Vector2f(-1,-1);
									//npc->setDeactivated(true);
								}
							}else{
								index--;
								int aux = index - 1;
								if(aux >= 0){
									wActual = n[index].node;
						    		wNext = n[index-1].node;
						    		toDoorW1 = true;
								}else if(index - 1 >= 0 && (n[0].node.getX() == wNext.getX() && n[0].node.getY() == wNext.getY())){
									//We completed the route and let the enemies we have following us let do their thing
									std::vector<Enemy*>& en = d->getEnemies();
									for(long unsigned int i=0; i<en.size(); i++){
										float e_x = en[i]->getX();
										float e_z = en[i]->getZ();
										float er = d->wichRoomIAmAt(e_x, e_z);
										if(er == aux2 && en[i]->getType() == P_SOLDIER_ENTITY){
											P_Soldier* psoldier = dynamic_cast<P_Soldier*> (en[i]);
											psoldier->setFollow(false);
											psoldier->setFollowUp(-1);
										}
									}
									inverse = false;
									wActual = Vector2f(-1,-1);
									fox->removePathList();
									SMemory* memory = npc->getMemory();
									SMemory_Slot* slot = memory->getSlotPerID(Player::Instance()->getID());
									slot->setChanged(false);
									slot = nullptr;
									memory = nullptr;
								}
							}
						}
						return true;
		    		}
		    	}else{
		    		std::vector<Enemy*>& en = d->getEnemies();
					for(long unsigned int i=0; i<en.size(); i++){
						float e_x = en[i]->getX();
						float e_z = en[i]->getZ();
						float er = d->wichRoomIAmAt(e_x, e_z);
						if(er == aux2 && en[i]->getType() == P_SOLDIER_ENTITY){
							P_Soldier* psoldier = dynamic_cast<P_Soldier*> (en[i]);
							psoldier->setFollow(false);
							psoldier->setFollowUp(-1);
						}
					}
					inverse = false;
					wActual = Vector2f(-1,-1);
					fox->removePathList();
					SMemory* memory = npc->getMemory();
					SMemory_Slot* slot = memory->getSlotPerID(Player::Instance()->getID());
					slot->setChanged(false);
					//npc->setDeactivated(true);
					slot = nullptr;
					memory = nullptr;

					return true;
		    	}
	    	}
	    }
    }

    return false;
}

void MoveToNode::seek(Door* door, Enemy* npc, Vector3f w, int a){
	npc->seek(w);
	if((int)abs(npc->getX() - w.getX()) < 1 && (int)abs(npc->getZ() - w.getZ()) < 1){
		if(a == 1){
			toDoorW1 = false;
			toDoorW2 = true;
			if(door->getLocked()){
				tolock = true;
				door->setLocked(false);
			}
			if(door->getClosed()){
				toclose = true;
				door->setClosed(false);
			}
			door->setInamovible(true);
		}else{
			toDoorW2 = false;
			if(tolock){
				tolock = false;
				door->setLocked(true);
			}
			if(toclose){
				toclose = false;
				door->setClosed(true);
			}
			door->setInamovible(false);
		}
	}
}
