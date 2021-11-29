#include "Camera.h"
#include "Motor3D.h"

Camera::Camera(float _z){
	state = 0;
    ps_x = 75;
	ps_y = 100;
	ps_z = -40;
	last_z = _z;
    velx = velz = 0;
}

Camera::~Camera(){
}

void Camera::setCameraPosition(float pl_x, float pl_y, float pl_z){
    ps_x = pl_x;
    ps_y = pl_y + 60;
    ps_z = pl_z - 80;

    m3D::Motor3D::Instance()->updatePlCamera(Vector3f(ps_x, ps_y, ps_z), Vector3f(pl_x, pl_y, pl_z));
}

void Camera::updateCamera(float pl_x, float pl_y, float pl_z){

	auto* m = m3D::Motor3D::Instance(); 

	switch(state){
        case 0: //arriba
            ps_x = pl_x;
            if(ps_y != pl_y + 60)
                ps_y = pl_y + 60;
            ps_z = pl_z - 80;
        break;
        case 1:{ //transicion abajo
            ps_x = pl_x;
        	bool change = true;
            if(ps_y > pl_y + 25){
                ps_y -= 25 * m->getDeltaTime();
                change = false;
            }
            if(ps_z < pl_z - 60 && ps_z < pl_z){
                ps_z += 25 * m->getDeltaTime();
                change = false;
            }else{
            	ps_z = pl_z - 60;
            	change = false;
            }
            if(change){
            	state = 2;
            }
        	break;
    	}
        case 2: //abajo
            ps_x = pl_x;
            if(ps_y != pl_y + 25)
                ps_y = pl_y + 25;
            ps_z = pl_z - 60;
        break;
        case 3:{ //transicion arriba
            ps_x = pl_x;
        	bool change = true;
            if(ps_y < pl_y + 60){
                ps_y += 25 * m->getDeltaTime();
                if(ps_y > pl_y + 50){
                	float aux = ps_y - (pl_y + 50);
                	ps_y = ps_y - aux * m->getDeltaTime();
                }
                change = false;
            }
           	if(ps_z > pl_z - 10){
                ps_z -= 25 * m->getDeltaTime();
                change = false;
            }
            if(ps_z < pl_z - 80){
            	float aux = pl_z - 80 - ps_z;
            	ps_z = ps_z + aux * m->getDeltaTime();
            }
            if(last_z < pl_z && ps_z < pl_z - 50){
            	ps_z += 80 * m->getDeltaTime();
            	change = false;
            }else if(last_z > pl_z){
            	ps_z -= 125 * m->getDeltaTime();
            	change = false;
            }
            if(change){
            	state = 0;
            }
        	break;
    	}
        case 4:{ //transicion hacia abajo para dialogo
            bool change = true;
            if(ps_y > pl_y + 25){
                ps_y -= 25 * m->getDeltaTime();
                change = false;
            }
            if(ps_x > pl_x - 15){
                ps_x -= 15 * m->getDeltaTime();
                change = false;
            }
            if(ps_z < pl_z - 50 && ps_z < pl_z){
                ps_z += 25 * m->getDeltaTime();
                change = false;
            }else{
                ps_z = pl_z - 50;
                change = false;
            }
            if(change){
                state = 2;
            }
            break;
        }
    }

    last_z = pl_z;

	m3D::Motor3D::Instance()->updatePlCamera(Vector3f(ps_x, ps_y, ps_z), Vector3f(pl_x, pl_y, pl_z));
}

void Camera::setCameraState(int st){
    if((state == 1 || state == 2) && st == 0){
        //do nothing
    }else{ 
        state = st;
    }
}

int Camera::getCameraState(){
	return state;
}

float Camera::getX(){
    return ps_x;
}

float Camera::getZ(){
    return ps_z;
}

float Camera::getY(){
    return ps_y;
}