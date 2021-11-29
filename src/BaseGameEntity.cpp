#include "BaseGameEntity.h"

int BaseGameEntity::m_nextValidID = 0;

BaseGameEntity::BaseGameEntity(){
	setID();
}

BaseGameEntity::~BaseGameEntity()
{
    //dtor
}

void BaseGameEntity::setID(){

	m_ID = m_nextValidID;

	m_nextValidID += 1;
}

float BaseGameEntity::getX(){
	return ps_x;
}

float BaseGameEntity::getY(){
	return ps_y;
}

float BaseGameEntity::getZ(){
	return ps_z;
}

void BaseGameEntity::setX(float _x){
	ps_x = _x;
}

void BaseGameEntity::setY(float _y){
	ps_y = _y;
}

void BaseGameEntity::setZ(float _z){
	ps_z = _z;
}