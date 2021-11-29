#ifndef OPTIONS_H
#define OPTIONS_H

#include "Estado.h"

class Options : public Estado
{
    public:
        static Options* Instance();
        void handleInput();
        void draw();
        void update();
        void clearState();
        void addEvents();
        void exit();
        void selectOption();
        void upOption();
        void downOption();
        void leftOption();
        void rightOption();
        void setClock(float);
        void setDrawed(bool);
    protected:
        Options();
        virtual ~Options();
    private:
        bool drawed;
        bool redraw;
        bool pulsed;
        bool left;
        bool up;
        int option;
        int option2;
        float then;
        float clock;
        bool audioon;
        float audiolevel;
        int resolution;
        bool fullscreen;
};

#endif // MENU_H