#include "Queen.h"
#include "Dungeon.h"
#include "Motor3D.h"


/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

Queen::Queen(Vector3f pos, int dia, int maxdial){

    //Characteristics
    velocidadX = 0.f;
    velocidadZ = 0.f;
    width = 70;
    height = 60;
    ps_x = pos.getX();
    ps_y = pos.getY();
    ps_z = pos.getZ();
    ini_x = pos.getX();;
    ini_y = pos.getY();;
    ini_z = pos.getZ();;
    dialog = dia;
    maxdialog = maxdial;
    inidialog = dia;
    state = 0;

    setType(QUEEN_ENTITY);

    //Physics
    PhysicsEngine::Instance()->createExistingShape(ps_x+17,ps_y,ps_z,10.0f,15.0f,10.0f,0.0f,rigidBody,phy_state,bodyShape);
    //PhysicsEngine::Instance()->createExistingCapsule(ps_x + 20,ps_y,ps_z,0.17,15,10.f,rigidBody,phy_state,bodyShape);
}

Queen::~Queen()
{
    if(rigidBody){
        delete rigidBody;
        rigidBody = nullptr;
    }
    if(phy_state){
        delete phy_state;
        phy_state = nullptr;
    }
    if(bodyShape){
        delete bodyShape;
        bodyShape = nullptr;
    }

    m3D::Motor3D::Instance()->clearNode(soldier);
    m3D::Motor3D::Instance()->clearNode(shadow);
}


/////////////////////////////////////////
// MAIN METHODS  
/////////////////////////////////////////

void Queen::updateEntity(){
    
    //Update State/Action Memory
    //updatePEntities();

}

