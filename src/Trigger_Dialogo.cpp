#include "Trigger_Dialogo.h"
#include "TriggerRegion_Rectangle.h"
#include "EventManager.h"
#include "Player.h"
#include "Motor3D.h"
#include "Dialog_State.h"

Trigger_Dialogo::Trigger_Dialogo(){
	setInactive();
	setToRemove(false);
	setType(TRIGGER_DIALOGO);
	touched = false;
    chat = 0;
}

Trigger_Dialogo::~Trigger_Dialogo(){
	delete m_regionOfInfluence;
	m_regionOfInfluence = nullptr;
}

void Trigger_Dialogo::Try(){
	auto* pl = Player::Instance();

	if(m_regionOfInfluence != nullptr){
		if(m_regionOfInfluence->isTouching(Vector2f(pl->getX(), pl->getZ()))){
            touched = true;
			auto* d = Dialog_State::Instance();
			if(chat == 9){
				d->setParams(chat, 2, P_KPONGO_ENTITY);
			}else{
            	d->setParams(chat, 2, TRIGGER_DIALOGO);
            }
            Game::Instance()->setState(Estado::StateType::DIALOG_STATE);
		}
	}
}

void Trigger_Dialogo::updateEntity(){
	if(touched){
		setToRemove(true);
	}
	else{
		Try();
	}
}

void Trigger_Dialogo::setChat(int c){
	chat = c;
}