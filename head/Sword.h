#ifndef Sword_H
#define Sword_H

#include "Wearable.h"

class Sword: public Wearable
{
    public:
        Sword();
        virtual ~Sword();
        void updateEntity();
        void playAudio(int);
    private:
};

#endif // Sword_H