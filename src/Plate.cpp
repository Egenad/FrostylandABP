#include "Plate.h"
#include "Player.h"
#include "EventManager.h"
#include "Dungeon.h"
#include "Room.h"
#include "AudioManager.h"

/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

Plate::Plate(float c_x, float c_y, float c_z, EntityType t, int re){
    ps_x = c_x;
    ps_y = c_y;
    ps_z = c_z;
    r = 15.f;
    setType(PRESSED_INTERACTIBLE);
    setRadiusOfInteract(20);
    interactibleType = t;
    required = re;
    pushed = false;
}

Plate::~Plate()
{
    //dtor
    m3D::Motor3D::Instance()->clearNode(object);
}

/////////////////////////////////////////
// UPDATE
/////////////////////////////////////////

void Plate::updateEntity(){
    auto* pl = Player::Instance();
    auto* d = Dungeon::Instance();

    float lvlm = d->getLevelMediaY() - 5;

	if(interactEntity(pl->getX(), pl->getZ())){
        if(ps_y != lvlm){
            //The player is standing above the plate
            pushed = true;
            playAudio(1);
            ps_y = lvlm;
            if(interactibleType == PLATE_1_2_INTERACTIBLE){
                EventManager::Instance()->addEvent(DOOR_1_2_PRESSED);
            }
        }
    }else{

        int aux = d->wichRoomIAmAt(ps_x, ps_z);
        Room* r = d->getRoomPerID(aux);

        if(r->checkCollisionWithBox(this)){
            if(ps_y != lvlm){
                //The box is standing above the plate
                pushed = true;
                playAudio(2);
                ps_y = lvlm;
                if(interactibleType == PLATE_1_2_INTERACTIBLE && required == r->IDwichCollides(this)){
                    EventManager::Instance()->addEvent(DOOR_1_2_PRESSED);
                }
            }
        }else{
            if(ps_y != lvlm + 5){
                pushed = false;
                playAudio(1);
                ps_y = lvlm + 5;
                if(interactibleType == PLATE_1_2_INTERACTIBLE){
                    EventManager::Instance()->addEvent(DOOR_1_2_RELEASED);
                }
            }
        }
    }

    m3D::Motor3D::Instance()->setPositionEntity(object,ps_x,ps_y,ps_z);
}

void Plate::playAudio(int a){

    auto* am = AudioManager::Instance();

    if(a == 1){
        am->shootAudio("HardFX/Objetos/Placa", -1, 0);;
    }else if (a == 2){
        am->shootAudio("HardFX/Objetos/Placa", -1, 0);;
        am->shootAudio("Diseño/Juego/PuzzleResuelto", -1, 0);
    }

    am = nullptr;

 }

int Plate::getRequired() const{
    return required;
}

bool Plate::getPushed() const{
    return pushed;
}

