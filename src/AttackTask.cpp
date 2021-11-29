#include "AttackTask.h"
#include "Enemy.h"
#include "Player.h"
#include <math.h>
#include "Dungeon.h"
#include <iostream>
#include "Motor3D.h"
#include "EventManager.h"

AttackTask::AttackTask(){
    then = m3D::Motor3D::Instance()->getNow() + 1;
}

AttackTask::~AttackTask(){

}

bool AttackTask::execute(int id){
	return attack(id); 
}



bool AttackTask::attack(int id){

    auto* d = Dungeon::Instance();
    auto* pl = Player::Instance();
    auto* m = m3D::Motor3D::Instance();
	Enemy* npc = d->getEnemyPerID(id);
    npc->setState(2);
	int r = 40;

    float pl_x = pl->getX();
    float pl_z = pl->getZ();

    float ps_x = npc->getX();
    float ps_z = npc->getZ();

    //Check distance between door and player

    float x = ps_x - pl_x;
    x = pow(x,2);
    float z = ps_z - pl_z;
    z = pow (z,2);
    float xz = x+z;

    float dist = sqrt(xz);

    if(dist <= r && dist != 0 &&  then > 1){
        if(npc->getType() == P_GENERAL_ENTITY){
            pl->setPushed(1);
            if(pl_x <= ps_x){
                if(pl_z > ps_z){
                    pl->setPushedVelocity(0,1);
                }else{
                    pl->setPushedVelocity(1,1);
                }
            }else{
                if(pl_z > ps_z){
                    pl->setPushedVelocity(0,0);
                }else{
                    pl->setPushedVelocity(1,0);
                }
            }
        }
        then = 0;
    	npc->playAudio(0);
        pl->receiveDamage(npc->getAttack());
        npc->setClock();
        npc->setVelocityX(0.f);
        npc->setVelocityZ(0.f);
        npc->setDeactivated(true);
    }else{
        then += m->getDeltaTime();
    }

    m = nullptr;
    d = nullptr;
    pl = nullptr;
    npc = nullptr;

	return true;
}
