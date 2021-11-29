#ifndef P_General_H
#define P_General_H

#include "Enemy.h"

class P_General: public Enemy
{
    public:
        P_General(Vector3f);
        virtual ~P_General();

        //Main Methods
        void recalculatePath();
        void playAudio(int);
        void updateAnimation();

        //Update
        void  updateEntity();
        void  executeTask();

    private:
        void generateBehaviorTree();
        bool follow;
        Selector* follow_task;
        int followup;
};
#endif // P_General_H