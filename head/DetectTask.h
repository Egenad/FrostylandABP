#ifndef DETECTTASK_H
#define DETECTTASK_H

#include "Task.h"

class DetectTask : public Task
{
    public:
    	DetectTask();
    	virtual ~DetectTask();
        bool execute(int); 
    private:
};

#endif // DETECTTASK_H