#ifndef P_KPongo_H
#define P_KPongo_H

#include "Enemy.h"
#include "Strategy.h"

class P_KPongo: public Enemy
{
    public:
        P_KPongo(Vector3f);
        virtual ~P_KPongo();

        //Main Methods
        void playAudio(int);

        //Update
        void updateEntity();
        void recalculatePath();
        void updateAnimation();

        void startBattle();
        void setInitY(float);
    private:

        //Private main methods
        Strategy* strategy;
        bool  start;
        float init_y;
};

#endif // P_KPongo_H