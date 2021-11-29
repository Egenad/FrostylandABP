#ifndef SEAL_QUEST_H
#define SEAL_QUEST_H

#include "Villager.h"

class Seal_Quest : public Villager
{
    public:

        //Ctor - Dtor
        Seal_Quest(Vector3f, int, int,int);
        virtual ~Seal_Quest();

        //Update
        void  updateEntity();

        //GETTERS

        bool getFinished();

        //Main Methods

        void endQuest();
        
    private:

    int reward;
    bool finished;
};

#endif // SEAL_QUEST_H
