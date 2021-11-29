#include "Button.h"

Button::Button(float _x, float _y, float _z){
	ps_x = _x;
	ps_y = _y;
	ps_z = _z;

	trap = nullptr;

	setType(BUTTON_INTERACTIBLE);
}

Button::~Button(){
	if(trap != nullptr){
		delete trap;
	}
}

void Button::assignTrap(Trap* t){
	trap = t;
}

void Button::updateEntity(){
	if(trap != nullptr){
		
	}
}

void Button::playAudio(int a){
	
}

