#include "Decoration.h"
#include "Motor3D.h"
#include "AudioManager.h"

Decoration::Decoration(float x,float y,float z,EntityType t, int an){
	ps_x = x;
    ps_y = y;
    ps_z = z;
    setType(t);
    rigidBody = nullptr;
    state = nullptr;
    bodyShape = nullptr;
    angle = an;
    timer = 0;
    sprite = 0;
    sound=nullptr;

    if(t == DECORATION_TABLE){
    	PhysicsEngine::Instance()->createExistingShape(ps_x+10,ps_y,ps_z,20.0f,40.0f,20.0f,0.0f,rigidBody,state,bodyShape);
    }else if(t == DECORATION_BARREL || t == DECORATION_BOX || t == DECORATION_CHAIR || t == DECORATION_COLUMN
        || t == DECORATION_STATUE1 || t == DECORATION_STATUE2 || t == DECORATION_THRONE){
        PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,15.0f,40.0f,15.0f,0.0f,rigidBody,state,bodyShape);
    }
}

Decoration::~Decoration(){
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

    sound = nullptr;
    
    m3D::Motor3D::Instance()->clearNode(object);	
}

void Decoration::updateEntity(){
	//this entity does nothing
    if(getType() == DECORATION_SOLDIER){

        auto* m = m3D::Motor3D::Instance();

        if(timer >= 0.5){
            
            timer = 0;

            if(sprite == 0){
                m->setMeshTexture(object,"../assets/textures/psoldier_sleep1.png");
            }else if(sprite == 1){
                m->setMeshTexture(object,"../assets/textures/psoldier_sleep2.png");
            }else if(sprite == 2){
                m->setMeshTexture(object,"../assets/textures/psoldier_sleep3.png");
            }

            if(sprite == 3){
                sprite = 0;
            }else{
                sprite++;
            }
        }
        timer += m->getDeltaTime();
    }
}

m3D::Node& Decoration::getNode(){
    return object;
}

void Decoration::drawDecoration(){
	m3D::Motor3D::Instance()->drawDecoration(object,ps_x,ps_y,ps_z,angle,getType());
}

void Decoration::playAudio(int a){

    if(a == 1){
        if(getType() == DECORATION_TORCH){
            sound = AudioManager::Instance()->shoot3DAudio("HardFX/Objetos/Antorcha",-1,0, Vector3f(ps_x, ps_y, ps_z));
            sound->setVolume(0.3);
        }else if(getType() ==  DECORATION_SOLDIER){
            sound = AudioManager::Instance()->shoot3DAudio("Voces/AI/Psoldado/Dormir",-1,0, Vector3f(ps_x, ps_y, ps_z));
        } 
    }else if(a == 0){
        if(sound){
            AudioManager::Instance()->stop(sound);
        }
    }
}