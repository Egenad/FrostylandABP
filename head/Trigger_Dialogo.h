#ifndef TRIGGER_DIALOGO_H
#define TRIGGER_DIALOGO_H

#include "Trigger.h"

class Trigger_Dialogo: public Trigger
{
    public:
        Trigger_Dialogo();
        virtual ~Trigger_Dialogo();
        void Try();
        void updateEntity();
        void setChat(int);
    private:
    	bool touched;
        int chat;
};

#endif // TRIGGER_CAMERA_H