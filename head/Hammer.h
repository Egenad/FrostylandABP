#ifndef Hammer_H
#define Hammer_H

#include "Wearable.h"

class Hammer: public Wearable
{
    public:
        Hammer();
        virtual ~Hammer();
        void  updateEntity();
        void  playAudio(int);
    private:
};

#endif // Hammer_H