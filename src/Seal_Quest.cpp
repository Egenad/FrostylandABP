#include "Seal_Quest.h"
#include "Dungeon.h"
#include "Motor3D.h"
#include "Player.h"
#include "Dialog_State.h"

/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

Seal_Quest::Seal_Quest(Vector3f pos, int dia, int maxdial, int rew){

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
    maxvel = 0.f;
    dialog = dia;
    maxdialog = maxdial;
    inidialog = dia;
    state = 0;
    reward = rew;
    finished = false;
}

Seal_Quest::~Seal_Quest()
{
    m3D::Motor3D::Instance()->clearNode(soldier);
    m3D::Motor3D::Instance()->clearNode(shadow);
}

/////////////////////////////////////////
// GETTERS 
/////////////////////////////////////////

bool Seal_Quest::getFinished(){
    return finished;
}



/////////////////////////////////////////
// MAIN METHODS  
/////////////////////////////////////////

void Seal_Quest::updateEntity(){
    //Update State/Action Memory
    updatePEntities();
}

void Seal_Quest::endQuest(){            //AL ACABAR DEL TODO BORRO AL NPC?¿?¿?¿
    auto * pl = Player::Instance();
    pl->addMoney(reward);
    finished = true;
    inidialog =  inidialog + maxdialog + 1;
    dialog = inidialog;
    maxdialog = 0;
}