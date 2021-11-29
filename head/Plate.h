#ifndef PLATE_H
#define PLATE_H

#include "Interactible.h"

class Plate: public Interactible
{

    public:
        Plate(float,float,float,EntityType,int);
        virtual ~Plate();
        void updateEntity();
        void addEvent();
        void playAudio(int);
        int getRequired() const;
        bool getPushed() const;
    private:
    	EntityType interactibleType;
        int required;
        bool pushed;
};

#endif // PLATE_H