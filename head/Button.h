#ifndef BUTTON_H
#define BUTTON_H

#include "Interactible.h"
#include "Trap.h"

class Button: public Interactible
{
    public:
        Button(float,float,float);
        virtual ~Button();
        void updateEntity();
        void assignTrap(Trap*);
        void playAudio(int);
    private:
    	Trap* trap;
};

#endif // BUTTON_H