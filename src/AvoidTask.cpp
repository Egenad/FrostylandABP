#include "AvoidTask.h"
#include "Enemy.h"
#include "Dungeon.h"
#include "Player.h"

AvoidTask::AvoidTask(){}

AvoidTask::~AvoidTask(){}

bool AvoidTask::execute(int id){
	return avoid(id);
}

bool AvoidTask::avoid(int id){
	auto* pl = Player::Instance();
    auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);

    if(npc){

        float player_x = pl->getX();
        float player_z = pl->getZ();

        float ps_x = npc->getX();
        float ps_z = npc->getZ();

        float distx = ps_x - player_x;
        float distz = ps_z - player_z;
            distx = distx * distx;
            distz = distz * distz;

        float dist = distx + distz;
            dist = sqrt(dist);
        if(dist <= 60){
            float vect_x = player_x;
		    float vect_z = player_z;

		    float velocidadX = npc->getVelocityX();
		    float velocidadZ = npc->getVelocityZ();

		    if(abs(ps_x - vect_x) > 0 || abs(ps_z - vect_z) > 0){

		        if(vect_x > ps_x){
		            velocidadX = -velocidadX;
		            npc->setVisionVectorValues(1,0,0);
		        }else if(vect_x < ps_x){
		            velocidadX = abs(velocidadX);
		            npc->setVisionVectorValues(-1,0,0);
		        }

		        if(vect_z > ps_z){
		            velocidadZ = -velocidadZ;
		            npc->setVisionVectorValues(0,0,1);
		        }else if(vect_z < ps_z){
		            velocidadZ = abs(velocidadZ);
		            npc->setVisionVectorValues(0,0,-1);
		        }
		    }

		    //CALCULO EL  TAMAÑO DEL VECTOR X, Z DE DIRECCION
		    float dist_x = vect_x - ps_x;
		    float dist_z = vect_z - ps_z;

		    //LO NORMALIZO Y ADECUO A LA VELOCIDAD

		    float normalize = (dist_x*dist_x) + (dist_z*dist_z);
		    normalize = sqrt(normalize);

		    //lo multiplicamos por la velocidad

		    float maxvel = npc->getMaxVel();

		    float steering_x = (dist_x/normalize) * maxvel;
		    float steering_z = (dist_z/normalize) * maxvel;

		    //calculamos el steeering

		    steering_x = steering_x - velocidadX;
		    steering_z = steering_z - velocidadZ;

		    velocidadX += steering_x;
		    velocidadX = truncf(velocidadX);
		    velocidadZ += steering_z;
		    velocidadZ = truncf(velocidadZ);

		    npc->setVelocityX(-velocidadX);
		    npc->setVelocityZ(-velocidadZ);  
        }
    }
    return true;
}
