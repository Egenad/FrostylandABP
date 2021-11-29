#include "Enemy.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include "BaseGameEntity.h"
#include "Player.h"
#include "EventManager.h"
#include "Fox.h"

void Enemy::updatePEntities(){
	auto* d = Dungeon::Instance();
    /*std::vector<Door*> drs = d->getDoors();
	for(long unsigned int i=0; i<drs.size(); i++){
		if(drs[i]){
			if(drs[i]->getType() == NORMAL_DOOR_ENTITY && en_sensor.detectEntity(getID(), 
                    Vector3f(drs[i]->getX(), drs[i]->getY(), drs[i]->getZ()))){
				struct EntityFlag entity;
				entity.id = drs[i]->getID();
				entity.type = NORMAL_DOOR_ENTITY;
				entity.p_x = drs[i]->getX();
				entity.p_y = drs[i]->getY();
				entity.p_z = drs[i]->getZ();
				memory->reportEntities(entity);
			}
		}
	}*/

    /* For some reason some1 made the sensor to only work with the player :(
    std::vector<Enemy*> em = d->getEnemies();
    for(long unsigned int i=0; i<em.size(); i++){
        if(em[i]){
            if(em[i]->getType() == P_SOLDIER_ENTITY && en_sensor.detectEntity(getID(), 
                    Vector3f(em[i]->getX(), em[i]->getY(), em[i]->getZ()))){
                struct EntityFlag entity;
                entity.id = em[i]->getID();
                entity.type = P_SOLDIER_ENTITY;
                entity.p_x = em[i]->getX();
                entity.p_y = em[i]->getY();
                entity.p_z = em[i]->getZ();
                memory->reportEntities(entity);
            }
        }
    }

    Sensor doesnt work with other things than the player so i'm just gonna
    see if they are close each other in other cpp.
    */

    int aux2 = d->wichRoomIAmAt(ps_x, ps_z);

	auto* pl = Player::Instance();
    int aux = d->wichRoomIAmAt(pl->getX(), pl->getZ());

    if(aux == aux2){
    	if(en_sensor.detectEntity(getID(), Vector3f(pl->getX(), pl->getY(), pl->getZ()))){
    		struct EntityFlag entity;
    		entity.id = pl->getID();
    		entity.type = PLAYER_ENTITY;
    		entity.p_x = pl->getX();
    		entity.p_y = pl->getY();
    		entity.p_z = pl->getZ();
    		memory->reportEntities(entity);
    	}
    }

	pl = nullptr;
	d = nullptr;
    //en_sensor.detectEntity(getID(), Vector3f(0,0,0));
}

void Enemy::seek(Vector3f vect){

    auto* d = Dungeon::Instance();

    float vect_x = vect.getX();
    float vect_z = vect.getZ();

    int __x = 0;
    int __z = 0;

    if(abs(ps_x - vect_x) > 0 || abs(ps_z - vect_z) > 0){

        if(vect_x > ps_x){
            velocidadX = abs(velocidadX);
            __x = 1;
        }else if(vect_x < ps_x){
            velocidadX = -velocidadX;
            __x = -1;
        }

        if(vect_z > ps_z){
            velocidadZ = abs(velocidadZ);
            __z = 1;
        }else if(vect_z < ps_z){
            velocidadZ = -velocidadZ;
            __z = -1;
        }

        setVisionVectorValues(__x,0,__z);
    }

    //CALCULO EL  TAMAÑO DEL VECTOR X, Z DE DIRECCION
    float dist_x = vect_x - ps_x;
    float dist_z = vect_z - ps_z;

    //LO NORMALIZO Y ADECUO A LA VELOCIDAD

    float normalize = (dist_x*dist_x) + (dist_z*dist_z);
    normalize = sqrt(normalize);

    //lo multiplicamos por la velocidad

    float steering_x = (dist_x/normalize) * maxvel;
    float steering_z = (dist_z/normalize) * maxvel;

    //calculamos el steeering

    steering_x = steering_x - velocidadX;
    steering_z = steering_z - velocidadZ;

    velocidadX += steering_x;
    velocidadX = truncf(velocidadX);
    velocidadZ += steering_z;
    velocidadZ = truncf(velocidadZ);   

    //HAGO UN GETNEARESTOBSTACLE Y SI ME DEVUELVE UN VECTOR LLAMO A AVOIDANCE 
    Vector3f obstpos = d->getNearestObst(ps_x, ps_z);
    if(obstpos.getX() != 0 && obstpos.getZ() != 0){
        Avoidance(obstpos);
    }
}

void Enemy::Avoidance(Vector3f vect){

    //ME PASAN EL VECTOR POSICION DEL OBJETO A ESQUIVAR
    //CALCULO LA DISTANCIA DE AMBOS OBJETOS

    float v_x = vect.getX();
    float v_z = vect.getZ();

    float distx = ps_x - v_x;
    float distz = ps_z - v_z;

    float dist = distx * distx + distz * distz;
    int rep = 20;
    //Y MIRO SI ESTA DENTRO DEL UMBRAL DE EMERGENCIA
    if(dist < 50){
        rep = 35;
    }
    dist = sqrt(dist);

    //CREO LA VISTA
    float vistax = (velocidadX/dist)*50;
    float vistaz = (velocidadZ/dist)*50;

    //HAGO EL AVOIDANCE  

    float avoidx = vistax - v_x;
    float avoidz = vistaz - v_z;

    //CALCULO LA FUERZA DE REPULSIÓN

    dist = avoidx * avoidx + avoidz * avoidz;
    
    avoidx = (avoidx/dist)*rep;
    avoidz = (avoidz/dist)*rep;

    //SUAVIZO EL STEERING
    avoidx = truncf(avoidx);
    avoidz = truncf(avoidz);

    //SE LO SUMO A LA VELOCIDAD

    velocidadX += avoidx;
    velocidadX = truncf(velocidadX);
    velocidadZ += avoidz;
    velocidadZ = truncf(velocidadZ);
}

void Enemy::Arrive(int id, int id2){
    Enemy* npc = Dungeon::Instance()->getEnemyPerID(id);
    Enemy* npc2 = Dungeon::Instance()->getEnemyPerID(id2);
    auto* pl = Player::Instance();

    float npc_x = npc->getX();
    float npc_z = npc->getZ();

    float npc2_x = npc2->getX();
    float npc2_z = npc2->getZ();

    float pl_x = pl->getX();
    float pl_z = pl->getZ();

    float slowrad = 25.f;
    float distx = 0;
    float distz = 0;
    if(id2 != 0){
        
        distx = pl_x - npc_x;
        distz = pl_z - npc_z;
    }else{
        
        slowrad = 35.f;
        distx = npc2_x - npc_x;
        distz = npc2_z - npc_z;
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

void Enemy::followTheLeader(int idp, int idz){
    Enemy* npcp = Dungeon::Instance()->getEnemyPerID(idp);
    Enemy* npcz = Dungeon::Instance()->getEnemyPerID(idz);
    Fox* fox = dynamic_cast<Fox*> (npcz);

    if(fox && npcp){       //SI ESTA EL ZORRO Y EL PINGUINO

        float fox_x = fox->getX();
        float fox_z = fox->getZ();

        float npcp_x = npcp->getX();
        float npcp_z = npcp->getZ();

        float dist = 0;
        float distx = npcp_x - fox_x;
        float distz = npcp_z - fox_z;
            distx = distx * distx;
            distz = distz * distz;
            dist = sqrt(distx + distz);
        
            float fox_vx = fox->getVelocityX();
            float fox_vz = fox->getVelocityZ();

        bool intenta = false;

        if(dist < 35 && ((fox_x < npcp_x && fox_vx > 0) || (fox_x > npcp_x && fox_vx < 0))){
            intenta = true;
            if(fox_z < npcp_z){
                npcp->setVelocityZ(50);
            }else{
                npcp->setVelocityZ(-50);
            }
        }
        if(dist < 35 && ((fox_z < npcp_z && fox_vz > 0) || (fox_z > npcp_z && fox_vz < 0))){
            intenta = true;
            if(npcp_x <= fox_x){
                npcp->setVelocityX(-50);
            }else{
                npcp->setVelocityX(50);
            }
        }
        if(!intenta){
                                    
            npcp->seek(Vector3f(fox_x,0.f,fox_z));        
            //Arrive(idp, idz);     

            /*distx = npcp->getVelocityX();
            distz = npcp->getVelocityZ();

            //OBTENEMOS LA POS DEL PINGUINO MAS CERCANO PAR QUE NO SE CHOQUEN SIGUIENDO AL ZORRO                    
            Vector3f separate = dung->getClosestPinguPos(idp);
            if(separate.getX() != 0 && separate.getZ() != 0){ //SI HAY MAS DE 1 PINGUINO
                //CALCULO EL  VECTOR DIRECCION
                distx = separate->getX() - npcp_x;
                distz = separate->getZ() - npcp_z;
                //LO NORMALIZO
                dist = (distx*distx) + (distz*distz);
                dist = sqrt(dist);
                
                //lo multiplicamos por la velocidad
                //y lo invertimospara quese repela
                distx = (distx/dist) * npcp->getMaxVel() * (-1);
                distz = (distz/dist) * npcp->getMaxVel() * (-1);
                //calculamos el desvio

                 distx = npcp->getVelocityX() + distx;
                 distz = npcp->getVelocityZ() + distz;
            }

            distx = truncf(distx);
            distz = truncf(distz); 
            
            //lo aplicamos
            npcp->setVelocityX(distx);
            npcp->setVelocityZ(distz);*/
        }
        npcp->setDeactivated(true);
    }
}

void Enemy::reduceLife(int d){
    auto* pl = Player::Instance();
    vida=vida-d;
    playAudio(1);
    if(death==false && vida < 0){
        Die();
        PhysicsEngine::Instance()->clearRigidBody(rigidBody);
    }else if(getType() != P_KPONGO_ENTITY){
        struct EntityFlag entity;
        entity.id = pl->getID();
        entity.type = PLAYER_ENTITY;
        entity.p_x = pl->getX();
        entity.p_y = pl->getY();
        entity.p_z = pl->getZ();
        memory->reportEntities(entity);
    } 
    animation=true;
}

void Enemy::setAnimState(int s){
    anim_state = s;
}

void Enemy::Die(){
    death = true;
    auto* m = m3D::Motor3D::Instance();
    Player::Instance()->setCoins(coins);
    m->clearNode(soldier);
    m->clearNode(shadow);
    playAudio(2);
}

bool Enemy::isDeath(){
    return death;
}

bool Enemy::detectEntity(Vector3f vect){
	return en_sensor.detectEntity(getID(), vect);
}

float Enemy::getVisionVectorX(){
    return vision_vector->getX();
}

void Enemy::setClock(){
    w_clock = m3D::Motor3D::Instance()->getNow() + 0.5;
}

float Enemy::getVisionVectorY(){
    return vision_vector->getY();
}

float Enemy::getVisionVectorZ(){
    return vision_vector->getZ();
}

bool Enemy::getchaseando(){
	return chaseando;
}

int Enemy::getHeardRatio(){
    return h_r;
}

int Enemy::getSightRatio(){
    return s_r;
}

bool Enemy::getDeath(){
    return death;
}

int Enemy::getAttack(){
	return ataque;
}

Path* Enemy::getPath(){
    return path;
}

SMemory* Enemy::getMemory(){
    return memory;
}

int Enemy::getSalaActual(){
    return salaActual;
}

int Enemy::getState(){
    return state;
}

float Enemy::getMass(){
    return mass;
}

int Enemy::getLife(){
    return vida;
}

//Setters

void Enemy::setPushVector(float x,float y,float z){
    push_vector.changeAllValues(x,y,z);
}

void Enemy::setVisionVectorX(float x){
	vision_vector->setX(x);
}

void Enemy::setVisionVectorY(float y){
	vision_vector->setY(y);
}

void Enemy::setVisionVectorZ(float z){
	vision_vector->setZ(z);
}

void Enemy::setVisionVectorValues(float x, float y, float z){
	vision_vector->changeAllValues(x,y,z);
}

void Enemy::setchaseando(bool b){
	chaseando = b;
}

void Enemy::setSalaActual(int sa){
    salaActual = sa;
}

void Enemy::setMaxLife(){
    vida = maxlife;
}

void Enemy::setState(int a){
    if(a >= 0 && a < 3){
        state = a;
    }
}

void Enemy::setMass(float m){
    mass=m;
}

void Enemy::setDeactivated(bool d){
    deactivated = d;
}