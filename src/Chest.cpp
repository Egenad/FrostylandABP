#include "Chest.h"
#include "Player.h"
#include "PhysicsEngine.h"
#include "EventManager.h"
#include "AudioManager.h"
#include "Dungeon.h"

/////////////////////////////////////////
// CONSTRUCTOR AND DESTRUCTOR
/////////////////////////////////////////

Chest::Chest(float c_x, float c_y, float c_z, EntityType t, int c){
    ps_x = c_x;
    ps_y = c_y;
    ps_z = c_z;
    r = 15.f;
    color = 'p';
    open = false;
    PhysicsEngine::Instance()->createExistingShape(ps_x,ps_y,ps_z,15.0f,20.0f,15.0f,0.0f,rigidBody,state,bodyShape);
    cant = c;
    setRadiusOfInteract(35);
    setType(CHEST_INTERACTIBLE);
    interactibleType = t;
}

Chest::~Chest()
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
    m3D::Motor3D::Instance()->clearNode(object);
}

void Chest::setColor(char c){
    color = c;
}

void Chest::updateEntity(){
	if(open == false){
        openChest();
    }
}

bool Chest::getOpen(){
    return open;
}

void Chest::setOpen(bool o){
    open = o;
}

EntityType Chest::getInteractibleType(){
    return interactibleType;
}

void Chest::openChest(){

    auto* pl = Player::Instance();
    auto* m = m3D::Motor3D::Instance();
    open = true;
    playAudio(0);
    //Cambio color
    //m->clearNode(object);
    //m->drawChest(object, ps_x, ps_y, ps_z);
    m->playAnimation(object);
    
    switch(interactibleType){

        case CHEST_FOOD_ICE_ENTITY:
            m->drawReceived(1);
            for(int i = 0; i<cant ; i++){
                pl->addFood(0);
            }
        break;

        case CHEST_FOOD_FISH_ENTITY:
            m->drawReceived(2);
            for(int i = 0; i<cant ; i++){
                pl->addFood(1);
            }
        break;

        case CHEST_FOOD_MEAT_ENTITY:
            m->drawReceived(3);
            for(int i = 0; i<cant ; i++){
                pl->addFood(2);
            }
        break;

        case CHEST_FOOD_BERRY_ENTITY:
            m->drawReceived(4);
            for(int i = 0; i<cant ; i++){
                pl->addFood(3);
            }
        break;

        case CHEST_NORMAL_KEY_ENTITY:
            m->drawReceived(5);
            pl->addKey();
        break;

        case CHEST_LEVEL_KEY_ENTITY:
            m->drawReceived(6);
            pl->setLevelKey(true);
        break;
        
        case CHEST_JAIL_KEY_ENTITY:
            m->drawReceived(7);
            pl->setJailKey(true);
        break;

        case CHEST_BOSS_KEY_ENTITY:
            m->drawReceived(8);
            pl->setBossKey(true);
        break;

        case CHEST_SWORD_ENTITY:
            m->drawReceived(9);
            pl->addWeapon(0);
        break;

        case CHEST_SLINGSHOT_ENTITY:
            m->drawReceived(10);
            pl->addWeapon(1);
        break;

        case CHEST_HAMMER_ENTITY:
            m->drawReceived(11);
            pl->addWeapon(2);
        break;
        default:
        break;
    }
    Dungeon::Instance()->startObjRec();
    ps_y = ps_y + 40;
    pl = nullptr;
    m = nullptr;
}

void Chest::playAudio(int a){

    if(a == 0){
        AudioManager::Instance()->shoot3DAudio("HardFX/Objetos/Cofre/Abrir", -1, 0, Vector3f(ps_x, ps_y, ps_z));
    }
}