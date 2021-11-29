#include "PatrolTask.h"
#include <iostream>
#include "Vector3f.h"
#include "Dungeon.h"
#include <math.h>
#include "tgmath.h"
#include "Enemy.h"

PatrolTask::PatrolTask(){

}

PatrolTask::~PatrolTask(){

}

bool PatrolTask::execute(int id){
    seek(id);
	return false; 
}


void PatrolTask::seek(int id){

    auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);

    //Si veo un pinguino patrullando intento esquivarlo
    if(npc){

        float ps_x = npc->getX();
        float ps_z = npc->getZ();

        bool avoid = false;

        int aux2 = d->wichRoomIAmAt(ps_x, ps_z);

        std::vector<Enemy*> em = d->getEnemies();
        for(long unsigned int i=0; i<em.size(); i++){
            if(em[i] && em[i]->getType() == P_SOLDIER_ENTITY && em[i]->getID() != npc->getID()){
                float en_x = em[i]->getX();
                float en_z = em[i]->getZ();
                int aux3 = d->wichRoomIAmAt(en_x, en_z);
                if(aux2 == aux3){
                    float x = en_x - ps_x;
                    x = pow(x,2);
                    float z = en_z - ps_z;
                    z = pow (z,2);
                    float xz = x+z;

                    float distance = sqrt(xz);

                    if(distance < 60){
                        avoid = true;
                        if(en_x > ps_x){
                            npc->setVelocityZ(60);
                        }else{
                            npc->setVelocityZ(-60);
                        }
                        if(en_z > ps_z){
                            npc->setVelocityX(60);
                        }else{
                            npc->setVelocityX(-60);
                        }
                    }
                }
            }
        }

        if(!npc->getPath()->empty() && !avoid){

        	Vector3f waypoint = npc->getPath()->getCurrentWayPoint();
            float vect_x = waypoint.getX();
            float vect_z = waypoint.getZ();

            float distx = vect_x - ps_x;
            float distz = vect_z - ps_z;
                distx = distx * distx;
                distz = distz * distz;
            float dist = distx + distz;
                dist = sqrt(dist);

            if(dist >= 5){
                npc->seek(waypoint);
            }else{
                npc->getPath()->SetNextWayPoint();
            }

            npc = nullptr;
            d = nullptr;
        }
    }
}