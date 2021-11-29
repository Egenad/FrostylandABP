#include "Node.h"

namespace m3D{
	Node::Node(){

	}

	Node::~Node(){

	}

	//scene::ISceneNode*& Node::getNode(){
	//	return node;
	//}

	ANode*& Node::getNode(){
		return node;
	}

	void Node::clearNode(){
		//node->remove();
		//node = nullptr;
	}

	void Node::setPosition(float x, float y, float z){
		//node->setPosition(core::vector3df(x,y,z));
	}
}