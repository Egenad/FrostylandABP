#ifndef P_SOLDIER_H
#define P_SOLDIER_H

#include "Enemy.h"

class P_Soldier : public Enemy
{
    public:

        //Ctor - Dtor
        P_Soldier(Vector3f);
        virtual ~P_Soldier();

        //Main Methods
        void recalculatePath();
        void playAudio(int);

        //Update
        void  updateEntity();
        void  executeTask();
        void  updateAnimation();

        //Getters
        bool getFollow();
        int  getFollowUp();
        //Setters
        void setFollow(bool);
        void setFollowUp(int);
        
    private:

        //Private main methods
        void generateBehaviorTree();
        bool follow;
        Selector* follow_task;
        int followup;
};

#endif // P_SOLDIER_H
