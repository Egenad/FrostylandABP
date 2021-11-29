#ifndef SEAL_NORMAL_H
#define SEAL_NORMAL_H

#include "Villager.h"

class Seal_Normal : public Villager
{
    public:

        //Ctor - Dtor
        Seal_Normal(Vector3f, int, int);
        virtual ~Seal_Normal();

        //Update
        void  updateEntity();
        
    private:
};

#endif // SEAL_NORMAL_H
