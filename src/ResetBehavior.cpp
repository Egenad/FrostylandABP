#include "ResetBehavior.h"
#include "Enemy.h"
#include "P_Soldier.h"
#include "Dungeon.h"

ResetBehavior::ResetBehavior(){}

ResetBehavior::~ResetBehavior(){

}

bool ResetBehavior::execute(int id){
	
	auto* d = Dungeon::Instance();
	Enemy* e = d->getEnemyPerID(id);

	P_Soldier* psoldier = dynamic_cast<P_Soldier*> (e);

	psoldier->setFollow(false);
	psoldier->setFollowUp(-1);

	return true; 
}
