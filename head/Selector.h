#ifndef SELECTOR_H
#define SELECTOR_H

#include "Task.h"

class Selector : public Task
{
    public:
    	Selector();
    	virtual ~Selector();
        bool execute(int); 
    private:
};

#endif // SELECTOR_H