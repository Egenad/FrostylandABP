#ifndef PATROLTASK_H
#define PATROLTASK_H

#include "Task.h"

class PatrolTask : public Task
{
    public:
        bool execute(int);
        PatrolTask();
        virtual ~PatrolTask();
    private:
    	void seek(int);
};

#endif // TASK_H