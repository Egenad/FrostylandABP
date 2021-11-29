#ifndef GAME_H
#define GAME_H

#include "Estado.h"

class Game
{
    public:
        static Game* Instance();
        void setState(Estado::StateType);
        void setClose(bool);
        void setCloseAll(bool);
        void runGame();
        void clearGame();
    protected:
        Game();
        Game(const Game& orig);
        virtual ~Game();
    private:
        Estado* state;
        bool close;
        bool closeall;

        float interpolation;
        float tick_time;
};

#endif // GAME_H
