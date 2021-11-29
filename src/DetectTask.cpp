#include "DetectTask.h"
#include "Enemy.h"
#include "Player.h"
#include "Dungeon.h"


DetectTask::DetectTask(){

}

DetectTask::~DetectTask(){

}

bool DetectTask::execute(int id){
    auto* pl = Player::Instance();
    auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);

    int aux = d->wichRoomIAmAt(pl->getX(), pl->getZ());
    int aux2 = d->wichRoomIAmAt(npc->getX(), npc->getZ());

    if(aux == aux2){    
        if(npc->detectEntity(Vector3f(pl->getX(), pl->getY(), pl->getZ()))){
            npc->setchaseando(true);
            npc->setState(1);
            npc = nullptr;
            pl = nullptr;
            return true;
        }
    }

    npc->setState(0);
    npc->setchaseando(false);
    npc = nullptr;
    pl = nullptr;
    
    return false;
}