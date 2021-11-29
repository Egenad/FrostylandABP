#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "Estado.h"
#include "FactoriaNivel.h"

class GameOver : public Estado
{
    public:
        static GameOver* Instance();
        void handleInput();
        void draw();
        void update();
        void clearState();
        void addEvents();
        void changeState(int);
        void getToMenu();
        void resetGame();
        void upOption();
        void downOption();
        void selectOption();
    protected:
        GameOver();
        virtual ~GameOver();
    private:
        static GameOver* pinstance;
        bool drawed;
        int option;
        float clock;
        bool redraw;
};

#endif // GameOver_H
