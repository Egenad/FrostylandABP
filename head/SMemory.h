#ifndef SMEMORY_H
#define SMEMORY_H

#include "BaseGameEntity.h"
#include <vector>
#include "SMemory_Slot.h"

class SMemory
{
    public:
        SMemory();
        virtual ~SMemory();
        //Getters
        std::vector<SMemory_Slot*>& GetAllPerceivedEntities();
        SMemory_Slot* getSlotPerID(int);
        //Methods
        void reportEntities(EntityFlag);
        bool checkEntityType(EntityType);

    private:
    	std::vector<SMemory_Slot*> p_entities;
};

#endif // SMEMORY_H