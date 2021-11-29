#include "Inventory.h"
#include "Motor3D.h"
#include "Game.h"
#include "Player.h"
#include "EventManager.h"
#include "AudioManager.h"
#include "Item_Selector.h"

using namespace m3D;

Inventory* Inventory::Instance(){
    static Inventory pinstance;
    return &pinstance;
}

void Inventory::handleInput(){

	auto* m = Motor3D::Instance();
	auto* e = EventManager::Instance();

	bool controller = false;
	//Joystick - Controller
    float x,y;
    if(m->isconectedJoystick()){
		controller=m->isconectedJoystick();
        m->initController();
        m->checkJoystickAxes(x,y);
    }

	if(then > 0.1){
		if(m->onKeyPressed('w') || (controller && (y<=-0.3f)))	e->addEvent(Event{Type::W_KEYBOARD_PRESSED});
		if(m->onKeyPressed('a') || (controller && (x<=-0.3f)))	e->addEvent(Event{Type::A_KEYBOARD_PRESSED});
		if(m->onKeyPressed('s') || (controller && (y>=0.3f)))	e->addEvent(Event{Type::S_KEYBOARD_PRESSED});
		if(m->onKeyPressed('d') || (controller && (x>=0.3f))) 	e->addEvent(Event{Type::D_KEYBOARD_PRESSED});
		then=0;
		

		if(e_released==true){
			if((controller && m->onButtonPressed('Y')) || m->onKeyPressed('i')){
				e_released=false;
				drawed=false;
				e->addEvent(Event{Type::E_KEYBOARD_PRESSED});
			}else if(m->onKeyPressed('i')){
				e_released=false;
				drawed=false;
				e->addEvent(Event{Type::E_KEYBOARD_PRESSED});
			}
		}else{
			if(controller){
				if(!m->onButtonPressed('Y') && !m->onKeyPressed('i')){
					e_released=true;
				}
			}else if(!m->onKeyPressed('i')){
				e_released = true;
			}
		}

		//for the item options
		if(p_released==true){
	        if(m->onKeyPressed('p') || (controller && m->onButtonPressed('B'))){
	            p_released=false;
	            drawed=false;
	        	AudioManager::Instance()->shootAudio("Diseño/Menu/SeleccionarOpcion",-1, 0);
	            e->addEvent(Event{Type::P_KEYBOARD_PRESSED});
	        }
	    }else{
	        if(!m->onKeyPressed('p') || (controller && !m->onButtonPressed('B'))){
	               p_released=true;
	        }
	    }
	}else{
		then += m->getDeltaTime();
	}

	m = nullptr;
	e = nullptr;
}

void Inventory::draw(){

	auto* m = Motor3D::Instance();

	if(drawed==false){
		drawImages();
		drawed=true;
	}

    //m->beginScene(true, true, 0, 200, 200, 200);
    m->drawScene();
    m = nullptr;
}

void Inventory::drawImages(){
	auto* m = Motor3D::Instance();
	auto* pl = Player::Instance();
	float x=box[selector].getX();
	float y=box[selector].getY();
	m->activarBillboards(Estado::StateType::INVENTORY_STATE);
	m->drawInventory(pl->getFood(), pl->getWeapons(), pl->getKeys(), pl->getBossKey(), pl->getLevelKey(), pl->getJailKey(), x, y, pl->getMoney());
	pl = nullptr;
	if(eikaState == 1){
		eikaState = -1;
		m->playEatingEika();
	}else if(eikaState == 0){
		eikaState = -1;
		m->playEquipEika();
	}
}

void Inventory::update(){
    EventManager::Instance()->runManager();
}

void Inventory::clearState(){

}

void Inventory::changeState(){
	auto* am = AudioManager::Instance();

    am->setVolume(am->getVolume()*2);
	am->shootAudio("Foleys/Inventario/Cerrar", -1, 0);

	Motor3D::Instance()->cleanDialogText();
	Motor3D::Instance()->activarBillboards(Estado::StateType::DUNGEON_STATE);
	Game::Instance()->setState(Estado::StateType::DUNGEON_STATE);
	e_released=false;
	p_released=true;
	selector = 0;
	drawed=false;
	am = nullptr;
}

void Inventory::addEvents(){
	auto* p = EventManager::Instance();

	p->addListener(E_KEYBOARD_PRESSED, [&](){changeState();});
	p->addListener(W_KEYBOARD_PRESSED, [&](){Inventory::Instance()->inventoryUp();});
	p->addListener(A_KEYBOARD_PRESSED, [&](){Inventory::Instance()->inventoryLeft();});
	p->addListener(S_KEYBOARD_PRESSED, [&](){Inventory::Instance()->inventoryDown();});
	p->addListener(D_KEYBOARD_PRESSED, [&](){Inventory::Instance()->inventoryRight();});
	p->addListener(P_KEYBOARD_PRESSED, [&](){Inventory::Instance()->inventorySelect();});
}

///////////////////////////////////////////
//// INVENTORY SELECTOR HANDLE
///////////////////////////////////////////

void Inventory::inventoryLeft(){
	if(selector>0){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
		selector--;
	}
	drawImages();
}

void Inventory::inventoryRight(){
	if(selector<GLOBAL_INVENTORY_SIZE-1){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
		selector++;
	}
	drawImages();
}	

void Inventory::inventoryUp(){
	if(selector>8){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
		selector=selector-6;
	}else if(selector<=8&&selector>2){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
		selector=2;
	}
	drawImages();
}

void Inventory::inventoryDown(){
	if(selector<3){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
		selector=3;
	}else if(selector<=26){
        AudioManager::Instance()->shootAudio("Diseño/Menu/Navegar",-1, 0);
		selector=selector+6;
	}
	drawImages();

}

void Inventory::inventorySelect(){

	int aux=selector;

	if(aux>=(GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE)){
		aux=-1;
	}else{
		if(aux<GLOBAL_WEAPONS_SIZE){ //is a weapon
			//check that the player has that weapon
			bool* pw = Player::Instance()->getWeapons();
			if(pw[aux]==false){
				aux=-1;
			}
		}else{ //is a food
			//check that the player has that food
			int rest=aux-GLOBAL_WEAPONS_SIZE;
			int* pf = Player::Instance()->getFood();
			if(pf[rest]==-1){
				aux=-1;
			}
		}
	}

	if(aux!=-1){
		//change the state to the item selector and send it the selected item
		Game::Instance()->setState(Estado::StateType::ITEM_SELECTOR_STATE);
		Item_Selector::Instance()->selectedItem(aux);
	}
}

const Vector2f Inventory::getPosition2d(int i){
	return box[i];
}

int Inventory::getItemSelected(){

	auto* pl = Player::Instance();
	int aux = selector;
	int item;
	if(aux>=GLOBAL_INVENTORY_SIZE){
		aux=-1;
		item = -1;

	}else{
		if(aux<GLOBAL_WEAPONS_SIZE){ 

			bool* armas = pl->getWeapons();
			//armas[aux];

			if(armas[aux]){
				switch(aux){
					case 0:	// SWORD
						item = 0;
					break;
					
					case 1:	//	SLINGSHOT
						item = 1;
					break;

					case 2:	// HAMMER
						item = 2;
					break;
					default:
						item = -1;
					break;
				}
			}else{
				item = -1;
			}
			
		}else{
			if(aux < GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE){ //is a food
				int* comida = pl->getFood();
					int a = comida[aux - GLOBAL_WEAPONS_SIZE]; 

					switch(a){

						case 0: // ICE
							item = 3;
						break;

						case 1:	// FISH
							item = 4;
						break;
						
						case 2:	// MEAT
							item = 5;
						break;

						case 3:	// BERRY
							item = 6;
						break;
						default:
							item = -1;
						break;
					}
			}

		}
		if(aux >= GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE){
			int a = -1;
			if(pl->getKeys() > 0){
				if( aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) < pl->getKeys()){
						a = 0;
				}else{

					if(pl->getLevelKey() && aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) == pl->getKeys()){
						a = 1;
					}
					if(pl->getLevelKey() == false && pl->getJailKey() && aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) == pl->getKeys()){
						a = 2;
					}
					if(pl->getLevelKey() == false && pl->getJailKey() == false && pl->getBossKey() && aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) == pl->getKeys()){
						a = 3;
					}
					if(pl->getLevelKey() && pl->getJailKey() && aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) == pl->getKeys() + 1){
						a = 2;
					}
					if(pl->getBossKey() && pl->getLevelKey() && pl->getJailKey() && aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) == pl->getKeys() + 2){
						a = 3;
					}
				}
			}else{
				if(pl->getLevelKey() && aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) == 0 ){
					a = 1;
				}
				if(pl->getJailKey() && aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) == 0 ){
					a = 2;
				}
				if(pl->getBossKey() && aux - (GLOBAL_FOOD_SIZE+GLOBAL_WEAPONS_SIZE) == 0 ){
					a = 3;
				}
			}
			switch(a){
				case 0: // KEY
					item = 7;
				break;
				case 1:	// LEVEL KEY
					item = 8;
				break;
				case 2:	// JAIL KEY
					item = 9;
				break;
				case 3:	// BOSS KEY
					item = 10;
				break;
				default:
					item = -1;
				break;
			}
		}
	}
	return item;
}



///////////////////////////////////////////
//// CTOR & DTOR
///////////////////////////////////////////

Inventory::Inventory()
{
	e_released=false;
	p_released=true;
	selector = 0;
	drawed=false;
	then=Motor3D::Instance()->getNow()+0.1;
	eikaState = -1;
}

Inventory::~Inventory(){

}

void Inventory::setEikaState(int i){
	eikaState = i;
}

