#ifndef ATTACKTASK_H
#define ATTACKTASK_H

#include "Task.h"

class AttackTask : public Task
{
    public:
        bool execute(int);
        AttackTask();
        virtual ~AttackTask();
    private:
    	bool attack(int);
    	float then;
};

#endif // ATTACKTASK_H