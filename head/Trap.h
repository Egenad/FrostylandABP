#ifndef TRAP_H
#define TRAP_H

#include "Object.h"
#include "TriggerRegion.h"

class Trap : public Object
{
    public:
        Trap(float,float,float,EntityType);
        virtual ~Trap();
        void updateEntity();
    private:
    	bool activated;
    	TriggerRegion* damageRegion;
};

#endif // BUTTON_H