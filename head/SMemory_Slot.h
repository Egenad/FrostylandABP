#ifndef SMEMORY_SLOT_H
#define SMEMORY_SLOT_H

#include "BaseGameEntity.h"
#include "Vector3f.h"

class SMemory_Slot
{
    public:
        SMemory_Slot(int, EntityType, Vector3f, int);
        virtual ~SMemory_Slot();
        //Setters
        void setParameters(float, float, float);
        void setChanged(bool);
        //Getters
        int getPriority();
        EntityFlag getEFlag();
        bool getChanged();
    private:
        EntityFlag entity;
    	int priority;
        bool changed;
};

#endif // SMEMORY_SLOT_H