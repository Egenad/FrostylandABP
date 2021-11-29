#include "FollowTask.h"
#include "Enemy.h"
#include "Dungeon.h"
#include "P_Soldier.h"

FollowTask::FollowTask(){}

FollowTask::~FollowTask(){}

bool FollowTask::execute(int id){
	return seek(id); 
}

bool FollowTask::seek(int id){
    auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);

    P_Soldier* psoldier = dynamic_cast<P_Soldier*> (npc);

    int idf = psoldier->getFollowUp();

    npc->followTheLeader(id, idf);

    return true;
}
