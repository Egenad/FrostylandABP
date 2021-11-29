#ifndef CHECKPLAYER_H
#define CHECKPLAYER_H

#include "Task.h"

class CheckPlayer : public Task
{
    public:
        bool execute(int);
        CheckPlayer();
        virtual ~CheckPlayer();
    private:
    	bool check(int);
};

#endif // CHASETASK_H