#ifndef CHASETASK_H
#define CHASETASK_H

#include "Task.h"

class ChaseTask : public Task
{
    public:
        bool execute(int);
        bool execute(int,int, int);
        ChaseTask();
        virtual ~ChaseTask();
        
    private:
    	bool Seek(int);
        void Arrive(int,int);
        void Avoidance(int,int);
        //void LeaderFollow(int,int);
};

#endif // CHASETASK_H