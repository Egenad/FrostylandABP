#ifndef TRIGGER_CAMERA_H
#define TRIGGER_CAMERA_H

#include "Trigger.h"

class Trigger_Camera: public Trigger
{
    public:
        Trigger_Camera();
        virtual ~Trigger_Camera();
        void Try();
        void updateEntity();
    private:
    	bool touched;
};

#endif // TRIGGER_CAMERA_H