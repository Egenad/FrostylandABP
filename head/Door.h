#ifndef Door_H
#define Door_H

#include "Interactible.h"
#include "Vector3f.h"
#include "PhysicsEngine.h"

class Door : public Interactible
{
    public:
        Door(float, float, bool, bool, EntityType);
        virtual ~Door();
        void updateEntity();
        //Setters
        void setClosed(bool);
        void setIDs(int,int);
        void setIndex(int,int);
        void setLocked(bool);
        void setInamovible(bool);
        void repositionRigidBody();
        //Getters
        bool getClosed();
        bool getLocked();
        bool getInamovible();
        int  getID1();
        int  getID2();
        int  getINDEX1();
        int  getINDEX2();
        bool getDown();
        Vector3f getWaypoints(int);
        Vector3f getPosition();
        //Main method
        void drawDoor();
        bool interactPlayer(float, float);
        void playAudio(int);
    private:
    	bool closed;
        bool locked;
        bool inamovible;
        bool down;
        Vector3f* w_1; 
        Vector3f* w_2;
        int r1;
        int r2;
        int index1;
        int index2;

        //Physics
        btRigidBody* rigidBody;
        btCollisionShape* bodyShape;
    	btDefaultMotionState* state;
};

#endif // Door_H