#ifndef DIALOG_STATE_H
#define DIALOG_STATE_H

#include "Estado.h"
#include "Globals.h"
#include "NPC.h"
#include "Game.h"

class Dialog_State : public Estado
{
    public:
        static Dialog_State* Instance();
        void draw();
        void update();
        void clearState();
        void changeState();
        void addEvents();
        void handleInput();
        void setParams(int,int, EntityType);
        void Check();
        
    protected:
        Dialog_State();
        ~Dialog_State();
        
    private:
        float then;
        EntityType tipo;
        int dial;
        int maxdial;
        bool drawed;
        bool p_released;
};

#endif // DIALOG_STATE_H