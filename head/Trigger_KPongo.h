#ifndef TRIGGER_KPONGO_H
#define TRIGGER_KPONGO_H

#include "Trigger.h"

class Trigger_KPongo: public Trigger
{
    public:
        Trigger_KPongo();
        virtual ~Trigger_KPongo();
        void Try();
        void updateEntity();
    private:
};

#endif // TRIGGER_KPONGO_H