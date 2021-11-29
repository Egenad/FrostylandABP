#ifndef MENU_H
#define MENU_H

#include "Estado.h"

class Menu : public Estado
{
    public:
        static Menu* Instance();
        void handleInput();
        void draw();
        void update();
        void clearState();
        void addEvents();
        void exit();
        void selectOption();
        void upOption();
        void downOption();
        void setClock(float);
        void setDrawed(bool);
        void resetGame();
    protected:
        Menu();
        virtual ~Menu();
    private:
        bool drawed;
        bool redraw;
        int option;
        float clock;
};

#endif // MENU_H
