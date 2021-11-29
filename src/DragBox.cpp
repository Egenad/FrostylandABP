#include "DragBox.h"
#include "Player.h"
#include "EventManager.h"
#include "Dungeon.h"
#include "PhysicsEngine.h"
#include "AudioManager.h"

/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

DragBox::DragBox(float c_x, float c_y, float c_z, int bid){
    ps_x = c_x;
    ps_y = c_y;
    ps_z = c_z;
    salaActual = -1;
    drawed = false;
    movement = -1;
    setType(DRAGGABLE_INTERACTIBLE);
    setRadiusOfInteract(45);
    boxid = bid;
    //Physics
    PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,23.5f,23.5f,20.0f,100000.f,rigidBody,state,bodyShape);
    Dungeon::Instance()->addDragBox(this);
    vel = 10;
}

DragBox::~DragBox(){
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
    //dtor
    m3D::Motor3D::Instance()->clearNode(object);
}

/////////////////////////////////////////
// UPDATE
/////////////////////////////////////////

void DragBox::updateEntity(){

    auto* d = Dungeon::Instance();
    auto* pl = Player::Instance();
    auto* ph = PhysicsEngine::Instance();
    //pl->setVelocity(50.0f);

    if(!drawed){
        float pl_x = pl->getX();
        float pl_z = pl->getZ();

        if(pl_z > ps_z + 34 || pl_z < ps_z - 34){ 
            pl->setMLeft(false);
            pl->setMRight(false);
            if(pl_z > ps_z + 34){   
                pl->setMUp(false);
                movement = 1;
            }else{
                pl->setMDown(false);
                movement = 0;
            }
            drawed = true;
            pl->setDraggeable(this);
        }else if(pl_x > ps_x + 34 || pl_x < ps_x - 34){
            pl->setMUp(false);
            pl->setMDown(false);
            if(pl_x > ps_x + 34){
                movement = 2;
                pl->setMRight(false);
            }else{
                movement = 3;
                pl->setMLeft(false);
            }
            drawed = true;
            pl->setDraggeable(this);
        } 
    }else{
        if(pl->allAnglesofMovement()){
            drawed = false;
            movement = -1;
        }else{
            //---------------BOX MOVEMENT------------------------
            float x=0;
            float z=0;
            vel = Player::Instance()->getVelocity() * Motor3D::Instance()->getDeltaTime() * 50;
            if(pl->getMoving()==true){
                playAudio(1);
                switch(movement){
                    case 0: //UP
                        z=vel;
                    break;
                    case 1: //DOWN
                        z=-vel;
                    break;
                    case 2: //LEFT
                        x=-vel;
                    break;
                    case 3: //RIGHT
                        x=vel;
                    break;
                }
            }else{
                playAudio(0);
            }
            ph->moveRigidBody(rigidBody,btVector3(x,0,z));
            /*std::cout << "X->" << x <<std::endl;
            std::cout << "Z->" << z <<std::endl;*/
        }
    }
    m3D::Motor3D::Instance()->setPositionEntity(object,ps_x,ps_y,ps_z);
    int aux = d->wichRoomIAmAt(ps_x, ps_z);
    
    if(salaActual != aux && aux != -1){
        if(salaActual != -1){
            //Erase object from the last room
            Room* r = d->getRoomPerID(salaActual);
            r->eraseInteractible(getID());
            //Add object to the new room
            salaActual = aux;
            r = d->getRoomPerID(salaActual);
            r->addinteractible(this);
        }
        salaActual = aux;
    }
}

void DragBox::moveDragBox(){
    auto* f = PhysicsEngine::Instance();
    f->moveDragBox(rigidBody, ps_x, ps_y, ps_z);

    btScalar rota=0.00;
    rigidBody->setAngularFactor(rota);
    //std::cout <<"POSICION FISICA: "<< ps_x <<" " << ps_y << " " << ps_z <<std::endl;
    if(movement==-1 || Player::Instance()->getInteractible()==nullptr){
        f->moveRigidBody(rigidBody,btVector3(0,0,0));   
        playAudio(0);
    }
    m3D::Motor3D::Instance()->setPositionEntity(object,ps_x,ps_y,ps_z);
}

void DragBox::reposition(float _x, float _z){
    PhysicsEngine::Instance()->reposition(_x, ps_y, _z, rigidBody, state);
}

void DragBox::playAudio(int a){

    if(a == 1){
        if( AudioManager::Instance()->isPlaying("HardFX/Objetos/Caja") == false){
            AudioManager::Instance()->shoot3DAudio("HardFX/Objetos/Caja", -1, 0, Vector3f(ps_x,ps_y,ps_z));
        }
    }else if(a == 0){
        if( AudioManager::Instance()->isPlaying("HardFX/Objetos/Caja") == true){
            AudioManager::Instance()->stop("HardFX/Objetos/Caja");
        }
    }

}
// GETTERS

int DragBox::getBoxId(){
    return boxid;
}
