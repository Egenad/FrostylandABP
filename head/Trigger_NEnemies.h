#ifndef TRIGGER_NENEMIES_H
#define TRIGGER_NENEMIES_H

#include "Trigger.h"
#include "Enemy.h"

class Trigger_NEnemies: public Trigger
{
    public:
        Trigger_NEnemies();
        virtual ~Trigger_NEnemies();
        void Try();
        void updateEntity();
        void setValor(int);
    private:
    	int valor;
};

#endif // TRIGGER_NENEMIES_H