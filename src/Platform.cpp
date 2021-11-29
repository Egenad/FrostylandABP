#include "Platform.h"
#include "Motor3D.h"
#include "Player.h"

/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

Platform::Platform(float c_x, float c_y, float c_z){
    ps_x = c_x;
    ps_y = c_y;
    ps_z = c_z;
    velocidadX = 0.f;
    velocidadZ = 0.f;
    maxVel = 40.f;
	mov = 0;
	med = 40.0f;
	velz = 80.0f;
	velx = 80.0f;
    setType(PLATFORM_ENTITY);
    PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,32.0f,1.5f,40.0f,0.f,rigidBody,state,bodyShape);
}

Platform::~Platform()
{
    //dtor
	if(rigidBody){
        delete rigidBody;
        rigidBody = nullptr;
    }
    if(state){
        delete state;
        state = nullptr;
    }
    if(bodyShape){
        delete bodyShape;
        bodyShape = nullptr;
    }
    if(path != nullptr){
    	delete path;
    	path = nullptr;
	}
	m3D::Motor3D::Instance()->clearNode(object);
}

/////////////////////////////////////////
// CLASS METHODS
/////////////////////////////////////////

void Platform::updateEntity(){

	auto* pl = Player::Instance();
	auto* f = PhysicsEngine::Instance();
	auto* m = m3D::Motor3D::Instance();

	float delta = m->getDeltaTime();

	if(path!=nullptr){

	    if(!path->empty()){

	    	Vector3f waypoint = path->getCurrentWayPoint();
	    	float vect_x = waypoint.getX();
    		float vect_z = waypoint.getZ();
	        if((int)abs(ps_x - waypoint.getX()) > 0 || (int)abs(ps_z - waypoint.getZ()) > 0){

	            if((int)abs(ps_x - vect_x) < 1){ //me muevo en Z
			        if(vect_z > ps_z){ //UP
			            velocidadZ = maxVel;
						
						if(pl->getX()<ps_x+med && pl->getX()>ps_x-med && pl->getZ()<ps_z+med && pl->getZ()>ps_z-med){
							float z = pl->getZ();
							z += velz*delta;
							if(mov==0){
								f->setPlayerPosition(pl->getX(), pl->getY(), z);
								mov = 1;
							}
							else{
								mov = 0;
							}
						}
			        }else if(vect_z < ps_z){ //DOWN
			            velocidadZ = -maxVel;
						if(pl->getX()<ps_x+med && pl->getX()>ps_x-med && pl->getZ()<ps_z+med && pl->getZ()>ps_z-med){
							float z = pl->getZ();
							z -= velz*delta;
							if(mov==0){
								f->setPlayerPosition(pl->getX(), pl->getY(),z);
								mov = 1;
							}
							else{
								mov = 0;
							}
						}
			        }
			        velocidadX = 0;
			    }else{ //me muevo en X
			    	if(vect_x > ps_x){ //RIGHT
			            velocidadX = maxVel;
						if(pl->getX()<ps_x+med && pl->getX()>ps_x-med && pl->getZ()<ps_z+med && pl->getZ()>ps_z-med){
							float x = pl->getX();
							x += velx*delta;
							if(mov==0){
								f->setPlayerPosition(x, pl->getY(), pl->getZ());
								mov = 1;
							}
							else{
								mov = 0;
							}
						}
			        }else if(vect_x < ps_x){ //LEFT
			            velocidadX = -maxVel;
						if(pl->getX()<ps_x+med && pl->getX()>ps_x-med && pl->getZ()<ps_z+med && pl->getZ()>ps_z-med){
							float x = pl->getX();
							x -= velx*delta;
							if(mov==0){
								f->setPlayerPosition(x, pl->getY(), pl->getZ());
								mov = 1;
							}
							else{
								mov = 0;
							}
						}

			        }
			        velocidadZ = 0;
			    }
	            
	        }else{
	            path->SetNextWayPoint();
	        }
		}

		ps_x += velocidadX * delta;
    	ps_z += velocidadZ * delta;

    	//object.setPosition(ps_x, ps_y, ps_z);
    	m->setPositionEntity(object,ps_x,ps_y,ps_z);
    	f->reposition(ps_x, ps_y, ps_z, rigidBody, state);
	}
}

void Platform::generatePath(float min_x, float min_z, float max_x, float max_z, bool loop){
	path = new Path(min_x, min_z, max_x, max_z, loop);
}

void Platform::seek(Vector3f vect){

	


}