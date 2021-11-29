#ifndef ANODE_H
#define ANODE_H

#include "AEntity.h"
#include <vector>

enum entityType{
    TRANSFORM,
    LIGHT,
    MESH,
    CAMERA
};

class ANode
{
    public:
        ANode();
        ANode(ANode* f, AEntity* e) : entity(e), father(f){}
        virtual ~ANode();
        void addChild(ANode*);
        void removeChild(ANode*);
        void setFather(ANode*);
        void setEntity(AEntity*);
        void setEntityType(entityType);
        AEntity* getEntity();
        ANode* getFather();
        void draw();
    private:
    	AEntity *entity;
    	std::vector<ANode*> childs;
    	ANode *father;
        entityType type;
};

#endif // ANODE_H