#ifndef TRIGGER_DOOR_H
#define TRIGGER_DOOR_H

#include "Trigger.h"

class Trigger_Door: public Trigger
{
    public:
        Trigger_Door();
        virtual ~Trigger_Door();
        void Try();
        void updateEntity();
    private:
};

#endif // TRIGGER_DOOR_H