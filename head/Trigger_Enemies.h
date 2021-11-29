#ifndef TRIGGER_ENEMIES_H
#define TRIGGER_ENEMIES_H

#include "Trigger.h"
#include "Enemy.h"

class Trigger_Enemies: public Trigger
{
    public:
        Trigger_Enemies();
        virtual ~Trigger_Enemies();
        void Try();
        void updateEntity();
    private:
};

#endif // TRIGGER_ENEMIES_H