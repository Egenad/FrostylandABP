#ifndef DRAGBOX_H
#define DRAGBOX_H

#include "Interactible.h"
#include "PhysicsEngine.h"

class DragBox: public Interactible
{

    public:
        DragBox(float,float,float,int);
		int getBoxId();
        virtual ~DragBox();
        void updateEntity();
		void moveDragBox();
        void reposition(float, float);
        void playAudio(int);
    private:
    	int salaActual;
    	bool drawed;
    	int movement; //0 = up; 1 = down; 2 = left; 3 = right
		int boxid;
		float vel;
    	btRigidBody* rigidBody;
    	btCollisionShape* bodyShape;
    	btDefaultMotionState* state;
};

#endif // DRAGBOX_H