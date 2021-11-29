#ifndef Slingshot_H
#define Slingshot_H

#include "Wearable.h"

class Slingshot: public Wearable
{
    public:
        Slingshot();
        virtual ~Slingshot();
        void  updateEntity();
        void playAudio(int);
    private:
};

#endif // Slingshot_H