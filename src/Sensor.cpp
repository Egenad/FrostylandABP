#include "Sensor.h"
#include <iostream>
#include "Dungeon.h"
#include "Player.h"

Sensor::Sensor(){

}

Sensor::~Sensor(){

}

bool Sensor::detectEntity(int id, Vector3f ent2){

    auto* pl = Player::Instance();
	Enemy* npc = Dungeon::Instance()->getEnemyPerID(id);

	float ang = 0.f;

    if(npc->getchaseando()){
        ang = M_PI/2;
    }else{
        ang = M_PI/6;
    }

    //Sight radio
    int s_r = npc->getSightRatio();
    //Heard radio
    int h_r = npc->getHeardRatio();

    int pl_x = ent2.getX();
    int pl_z = ent2.getZ();

    int npc_x = npc->getX();
    int npc_z = npc->getZ();

    //Check distance between npc and player
    float dist = pl->calculateDistanceEnemy(id);

    if(dist <= h_r){
        if(abs(npc_x - pl_x) > 0 || abs(npc_z - pl_z) > 0){

            return true;

        }
        ang = M_PI/2;
    }

    if(dist <= s_r && dist != 0){
        float ang_fin = pl->calculateAngleEnemy(id);

        if(ang_fin <= ang){

            npc = nullptr;

            return true;  
        }
    }
    
    npc->setchaseando(false);
    npc = nullptr;

	return false;
}