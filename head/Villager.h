#ifndef Villager_H
#define Villager_H

#include "NPC.h"
#include "Vector3f.h"
#include "PhysicsEngine.h"

class Villager: public NPC
{
    public:

        virtual ~Villager();
        

        //Main methods
        void updatePEntities();
        void callDialog(int);
        void playAudio(int);
        

        //Getters
        int  getDialog();
        int  getMaxDialog();
        int  getIniDialog();
        int  getState();

        //Setters
        void  resetVillager();
        void  setDialog(int);
        void  setMaxDialog(int);
        void  setIniDialog(int);
        void  setState(int);

    protected:
        int state;
        int dialog;
        int inidialog;
        int maxdialog;

        //Physics
        btRigidBody* rigidBody;
    	btCollisionShape* bodyShape;
    	btDefaultMotionState* phy_state;
};

#endif // Villager_H