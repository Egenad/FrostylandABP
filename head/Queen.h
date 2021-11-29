#ifndef QUEEN_H
#define QUEEN_H

#include "Villager.h"

class Queen : public Villager
{
    public:

        //Ctor - Dtor
        Queen(Vector3f, int, int);
        virtual ~Queen();

        //Update
        void  updateEntity();
        
    private:

    	btRigidBody* rigidBody;
    	btCollisionShape* bodyShape;
    	btDefaultMotionState* phy_state;
};

#endif // QUEEN_H