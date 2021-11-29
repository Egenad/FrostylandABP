#ifndef TRIGGER_FALL_H
#define TRIGGER_FALL_H

#include "Trigger.h"

class Trigger_Fall: public Trigger
{
    public:
        Trigger_Fall();
        virtual ~Trigger_Fall();
        void Try();
        void updateEntity();
    private:
};

#endif // TRIGGER_FALL_H