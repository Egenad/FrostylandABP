#include "Item_Selector.h"
#include "Motor3D.h"
#include "Game.h"
#include "Player.h"
#include "Inventory.h"
#include "EventManager.h"
#include "AudioManager.h"
#include "Globals.h"

using namespace m3D;

Item_Selector* Item_Selector::Instance(){
    static Item_Selector pinstance;
    return &pinstance;
}

void Item_Selector::handleInput(){

	auto* m = Motor3D::Instance();
	auto* e = EventManager::Instance(); 
	auto* am = AudioManager::Instance();

	bool controller = false;
	//Joystick - Controller
    float x,y;
    if(m->isconectedJoystick()){
		controller=m->isconectedJoystick();
        m->initController();
        m->checkJoystickAxes(x,y);
    }

	if(then>0.1){
		if(m->onKeyPressed('w') || (controller && (y<=-0.3f)))	e->addEvent(Event{Type::W_KEYBOARD_PRESSED});
		if(m->onKeyPressed('s') || (controller && (y>=0.3f)))	e->addEvent(Event{Type::S_KEYBOARD_PRESSED});
		then=0;

		if(p_released==true){
			if(m->onKeyPressed('p') || (controller && m->onButtonPressed('B'))){ 
				p_released=false;
				//m->clearInventory();
	        	am->shootAudio("Diseño/Menu/SeleccionarOpcion",-1, 0);
				e->addEvent(Event{Type::P_KEYBOARD_PRESSED});
			}
		}else{
			if(!m->onKeyPressed('p') || (controller && !m->onButtonPressed('B')))
				p_released=true;
		}
	}else{
		then += m->getDeltaTime();
	}

	m = nullptr;
	am = nullptr;
}

void Item_Selector::draw(){

	auto* m = Motor3D::Instance();

	if(drawed==false){
		Vector2f position = Inventory::Instance()->getPosition2d(item);
		m->drawItemOptions(position.getX(),position.getY(),selector,item);
		drawed=true;
	}

    //m->beginScene(true, true, 0, 200, 200, 200);
    m->drawScene();
    m = nullptr;
}

void Item_Selector::update(){
    EventManager::Instance()->runManager();
}

void Item_Selector::clearState(){
}

void Item_Selector::changeState(){
	selector=0;
	drawed=false;
	Game::Instance()->setState(Estado::StateType::INVENTORY_STATE);
	Motor3D::Instance()->activarBillboards(Estado::StateType::INVENTORY_STATE);
}

void Item_Selector::addEvents(){
	auto* p = EventManager::Instance();

	p->addListener(P_KEYBOARD_PRESSED, [&](){selectOption();});
	p->addListener(W_KEYBOARD_PRESSED, [&](){Item_Selector::Instance()->selectorUp();});
	p->addListener(S_KEYBOARD_PRESSED, [&](){Item_Selector::Instance()->selectorDown();});
}

///////////////////////////////////////////
//// OPTIONS SELECTOR HANDLE
///////////////////////////////////////////

void Item_Selector::selectorUp(){

	if(selector>0){
		AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
		selector--;
	}
	drawed=false;
	draw();
}

void Item_Selector::selectorDown(){

	if(item < GLOBAL_WEAPONS_SIZE){
		if(selector<1){
			AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
			selector++;
		}
	}else{
		if(selector<2){
			AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
			selector++;
		}
	}

	drawed=false;
	draw();
}

void Item_Selector::selectOption(){

	auto* pl = Player::Instance();
	switch(selector){
		case 0:
			//use
			if(item<GLOBAL_WEAPONS_SIZE){
				//equip
				pl->equipWeapon(item);
				Inventory::Instance()->setEikaState(0);
				changeState();
			}else{
				//eat
				auto* m = Motor3D::Instance();
				int aux=item-GLOBAL_WEAPONS_SIZE;
				int* pf = pl->getFood();
				int heal=0;
				switch(pf[aux]){
					case 0:
						heal=3;
						break;
					case 1:
						heal=5;
						break;
					case 2:
						heal=2;
						break;
					case 3:
						heal=1;
						break;
					
				}
				pl->heal(heal);
				pl->eraseFood(aux);
				//redraw the inventory and lifes to show that the player has healed his lifes
				m->insertLifes(pl->getLifes());
				Vector2f position = Inventory::Instance()->getPosition2d(item);
				m->drawInventory(pl->getFood(),pl->getWeapons(), pl->getKeys(), pl->getBossKey(), pl->getLevelKey(), pl->getJailKey(), position.getX(), position.getY(), pl->getMoney());
				Inventory::Instance()->setEikaState(1);
				changeState();
			}
			break;
			
		case 1:
			if(item < GLOBAL_WEAPONS_SIZE){
				//go back
				changeState();
			}else{
				//erase item
				int aux = item - GLOBAL_WEAPONS_SIZE;
				//sound
    			AudioManager::Instance()->shootAudio("Foleys/Objetos/Tirar", -1, 0);
				pl->eraseFood(aux);
				changeState();
			}

			break;
		case 2:
			//go back
			changeState();
			break;
	}
}

void Item_Selector::selectedItem(int i){
	item = i;
}

///////////////////////////////////////////
//// CTOR & DTOR
///////////////////////////////////////////

Item_Selector::Item_Selector()
{
	p_released=false;
	selector = 0;
	item = 0;
	drawed=false;
	then=Motor3D::Instance()->getNow()+0.1;
}

Item_Selector::~Item_Selector(){

}