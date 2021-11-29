#ifndef MOVETO_H
#define MOVETO_H

#include "Task.h"

class MoveTo : public Task
{
    public:
        bool execute(int);
        MoveTo();
        virtual ~MoveTo();
    private:
    	bool movetopoint(int);
    	bool check();
    	bool look;
    	int vect_change;
    	float then;
    	bool changed;
};

#endif // CHASETASK_H