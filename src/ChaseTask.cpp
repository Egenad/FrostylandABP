#include "ChaseTask.h"
#include "Player.h"
#include "Dungeon.h"
#include "Enemy.h"
#include "Fox.h"
#include "Vector3f.h"

ChaseTask::ChaseTask(){

}

ChaseTask::~ChaseTask(){

}


bool ChaseTask::execute(int id){
    Enemy* npc = Dungeon::Instance()->getEnemyPerID(id);
    npc->setState(1);
	return Seek(id); 
}

bool ChaseTask::execute(int id, int id2, int movetype){

    Enemy* npc = Dungeon::Instance()->getEnemyPerID(id);
    npc->setState(1);
    
    /*switch(movetype){

        case 1:
            return Seek(id);
            break;
        case 2:
            Seek(id);
            Avoidance(id,0);
            break;
        default:
        break;
    }*/
	return true; 
}

bool ChaseTask::Seek(int id){

    auto* pl = Player::Instance();
    auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);

    if(npc){
        float player_x = pl->getX();
        float player_z = pl->getZ();
        if(d->wichRoomIAmAt(player_x, player_z) != d->wichRoomIAmAt(npc->getX(), npc->getZ())){
            return false;
        }
        float _x = npc->getX();
        float _z = npc->getZ();

        float distx = _x - player_x;
        float distz = _z - player_z;
            distx = distx * distx;
            distz = distz * distz;
        float dist = distx + distz;
            dist = sqrt(dist);
        if(dist >= 50){
            npc->seek(Vector3f(player_x,0.f,player_z));
        }
    }
    return true;
}

void ChaseTask::Arrive(int id, int id2){
    
    Enemy* npc = Dungeon::Instance()->getEnemyPerID(id);
    float slowrad = 10.f;
    float distx = 0;
    float distz = 0;
    if(id2 != 0){
        auto* pl = Player::Instance();
        distx = pl->getX() - npc->getX();
        distz = pl->getZ() - npc->getZ();
    }else{
        Enemy* npc2 = Dungeon::Instance()->getEnemyPerID(id);
        slowrad = 15.f;
        distx = npc2->getX() - npc->getX();
        distz = npc2->getZ() - npc->getZ();
    }
    
    float velx = npc->getVelocityX();
    float velz = npc->getVelocityZ();

	if(abs(distx) <= slowrad){
		velx = velx * abs(distx) / slowrad;
        npc->setVelocityX(velx);
    }

	if(abs(distz) <= slowrad){
		velz = velz * abs(distz) / slowrad;
        npc->setVelocityZ(velz);
	}

    npc = nullptr;
}

void ChaseTask::Avoidance(int id1, int id2){}

/*void ChaseTask::LeaderFollow(int idp, int idz){
    auto* dung = Dungeon::Instance();
    Enemy* npcp = Dungeon::Instance()->getEnemyPerID(idp);
    Enemy* npcz = Dungeon::Instance()->getEnemyPerID(idz);
    Fox* fox = dynamic_cast<Fox*> (npcz);

    if(fox && npcp){       //SI ESTA EL ZORRO Y EL PINGUINO
        float dist = 0;
        float distx = npcp->getX() - fox->getX();
        float distz = npcp->getZ() - fox->getZ();
            distx = distx * distx;
            distz = distz * distz;
            dist = sqrt(distx + distz);
        if(dist >= 50){                                       //SI ESTAN A UNA DISTANCIA BIEN
            Vector3f* foxpos = fox->getLastPosition();
            npcp->seek(Vector3f(foxpos->getX(),0.f,foxpos->getZ()));        //SIGUE A LA POS ANTERIOR DEL ZORRO
            Arrive(idp, idz);                                                           //USA ARRIVE PARA NO PASARSE DE RAPIDO
            
            //OBTENEMOS LA POS DEL PINGUINO MAS CERCANO PAR QUE NO SE CHOQUEN SIGUIENDO AL ZORRO                    
            Vector3f* separate = dung->getClosestPinguPos(idp);
            
            //CALCULO EL  VECTOR DIRECCION
            distx = separate->getX() - npcp->getX();
            distz = separate->getZ() - npcp->getZ();

            //LO NORMALIZO
            dist = (distx*distx) + (distz*distz);
            dist = sqrt(dist);

            //lo multiplicamos por la velocidad
            //y lo invertimospara quese repela
            distx = (distx/dist) * npcp->getMaxVel() * (-1);
            distz = (distz/dist) * npcp->getMaxVel() * (-1);

            //calculamos el desvio

            distx= truncf(distx);
            distz = truncf(distz);    

            //lo aplicamos
            npcp->setVelocityX(npcp->getVelocityX() + distx);
            npcp->setVelocityZ(npcp->getVelocityZ() + distz);

        }else{
            npcp->setVelocityX(0);
            npcp->setVelocityZ(0);
        }
    }
}*/

