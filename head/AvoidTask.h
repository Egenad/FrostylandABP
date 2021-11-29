#ifndef AVOIDTASK_H
#define AVOIDTASK_H

#include "Task.h"

class AvoidTask : public Task
{
    public:
        bool execute(int);
        AvoidTask();
        virtual ~AvoidTask();
    private:
    	bool avoid(int);
};

#endif // AVOIDTASK_H