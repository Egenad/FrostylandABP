#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Task.h"

class Sequence : public Task
{
    public:
    	Sequence();
    	virtual ~Sequence();
        bool execute(int); 
    private:
};

#endif // SEQUENCE_H