#include "Seal_Normal.h"
#include "Dungeon.h"
#include "Motor3D.h"


/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

Seal_Normal::Seal_Normal(Vector3f pos, int dia, int maxdial){

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
}

Seal_Normal::~Seal_Normal()
{
    m3D::Motor3D::Instance()->clearNode(soldier);
    m3D::Motor3D::Instance()->clearNode(shadow);
}


/////////////////////////////////////////
// MAIN METHODS  
/////////////////////////////////////////

void Seal_Normal::updateEntity(){
    
    //Update State/Action Memory
    //updatePEntities();

}

