#ifndef PAUSE_H
#define PAUSE_H

#include "Estado.h"

class Pause : public Estado
{
    public:
        static Pause* Instance();
        void handleInput();
        void draw();
        void update();
        void clearState();
        void addEvents();
        void changeState();
        void upOption();
        void downOption();
        void selectOption();
        void getToMenu();
    protected:
        Pause();
        virtual ~Pause();
    private:
        bool drawed;
        bool redraw;
        int option;
        float clock;
};

#endif // PAUSE_H
