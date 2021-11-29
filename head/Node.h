#ifndef NODE_H
#define NODE_H

//#include "RenderFacade.h"
#include "ANode.h"

namespace m3D{
	class Node
	{
	    public:
	        Node();
	        virtual ~Node();
	    	//scene::ISceneNode*& getNode();
	    	ANode*& getNode();
	    	void clearNode();
	    	void setPosition(float, float, float);
	    private:
	    	//scene::ISceneNode* node;
	    	ANode* node;
	};
}

#endif // NODE_H