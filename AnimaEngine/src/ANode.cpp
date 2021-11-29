#include "ANode.h"
#include <iostream>

ANode::ANode(){
	entity = nullptr;
	father = nullptr;
}

ANode::~ANode(){
	if(entity!=nullptr){
		delete entity;
		entity = nullptr;
	}
	for(int i=0;i<childs.size();i++){
		if(childs[i]){
			delete childs[i];
			childs[i] = nullptr;
		}
	}
	father = nullptr;
}

void ANode::addChild(ANode* child){
	//Luke, I am your father
	child->setFather(this);
	childs.push_back(child);
}

void ANode::removeChild(ANode* child){
	int i;
	for(i=0;i<childs.size();i++){
		if(childs[i]==child) break;
	}
	delete childs[i];
	childs[i] = nullptr;
	childs.erase(childs.begin()+i);
}

void ANode::setFather(ANode* f){
	father = f;
}

void ANode::setEntity(AEntity* entity){
	this->entity = entity;
}

void ANode::setEntityType(entityType t){
	type = t;
}

AEntity* ANode::getEntity(){
	return entity;
}

ANode* ANode::getFather(){
	return father;
}

void ANode::draw(){
	if(entity!=nullptr)
		entity->beginDraw();

	for(int i=0;i<childs.size();i++)
		childs[i]->draw();

	if(entity!=nullptr)
	 entity->endDraw();
}