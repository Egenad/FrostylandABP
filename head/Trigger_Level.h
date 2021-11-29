#ifndef TRIGGER_LEVEL_H
#define TRIGGER_LEVEL_H

#include "Trigger.h"

class Trigger_Level: public Trigger
{
    public:
    	Trigger_Level();
        virtual ~Trigger_Level();
        void Try();
        void updateEntity();
        void setToLevel(int);
    private:
    	int toLevel;
};

#endif // TRIGGER_CAMERA_H