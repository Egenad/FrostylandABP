#include "Dialog_State.h"
#include "Motor3D.h"
#include "Game.h"
#include "EventManager.h"
#include "Player.h"

using namespace m3D;


Dialog_State* Dialog_State::Instance(){
    static Dialog_State pinstance;
    return &pinstance;
}

void Dialog_State::handleInput(){
    
	auto* m = Motor3D::Instance();
	auto* e = EventManager::Instance();

    bool controller=false;

    //Joystick - Controller
    if(m->isconectedJoystick()){
        controller=m->isconectedJoystick();
        m->initController();
    }

    if(p_released){
        if(m->onKeyPressed('p') || (controller && m->onButtonPressed('B'))){
            p_released=false;
            e->addEvent(Event{Type::P_KEYBOARD_PRESSED});
        }		  
    }else{
        if(controller){
            if(!m->onButtonPressed('B') && !m->onKeyPressed('p')){
                p_released = true;
            }
        }else if(!m->onKeyPressed('p')){
			p_released=true;
        }
	}
	m = nullptr;
    e = nullptr;
}

void Dialog_State::draw(){

	auto* m = Motor3D::Instance();

    if(drawed==false){
		m->activarBillboards(Estado::StateType::DIALOG_STATE);
        m->drawDialog(dial, tipo);
		drawed=true;
	}

    m->drawScene();
    m = nullptr;
}

void Dialog_State::update(){
    EventManager::Instance()->runManager();
    Player::Instance()->updateCamera();
}

void Dialog_State::clearState(){

}
  

void Dialog_State::Check(){
    
    if(dial < maxdial){
        p_released = false;
        drawed = false;
        dial++;                                 
        
    }else{
        p_released = false;
        dial = 0;
        maxdial = 0;
        drawed = false;
        changeState();
        
    }    
}

void Dialog_State::changeState(){
    auto* pl = Player::Instance();
    auto* m = m3D::Motor3D::Instance();
	Game::Instance()->setState(Estado::StateType::DUNGEON_STATE);
    m->activarBillboards(Estado::StateType::DUNGEON_STATE);
    m->cleanDialogText();
    pl->setPCameraState(0);
    pl->setInteractTime(m3D::Motor3D::Instance()->getNow()+1);
}

void Dialog_State::addEvents(){
	auto* p = EventManager::Instance();
	p->addListener(P_KEYBOARD_PRESSED, [&](){Check();});
}


///////////////////////////////////////////
//// CTOR & DTOR
///////////////////////////////////////////

Dialog_State::Dialog_State()
{
    maxdial = 0;
    dial = 0;
    drawed = false;
    tipo = P_KPONGO_ENTITY;
    p_released = false;
}

void Dialog_State::setParams(int val, int max, EntityType ti)
{
    maxdial = max + val;
    dial = val;
    tipo = ti;
}

Dialog_State::~Dialog_State(){}
