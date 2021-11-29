#ifndef Chest_H
#define Chest_H

#include "Interactible.h"
#include "PhysicsEngine.h"

class Chest: public Interactible
{
    public:
        Chest(float,float,float,EntityType,int);
        virtual ~Chest();
        void updateEntity();
        //methods
        void openChest();
        bool getOpen();
        void setOpen(bool);
        void setColor(char);
        EntityType getInteractibleType();
        void playAudio(int);
    private:
    	bool open;
        int cant;
    	EntityType interactibleType;
        btRigidBody* rigidBody;
        btCollisionShape* bodyShape;
        btDefaultMotionState* state;
};

#endif // Chest_H