#ifndef RESETBEHAVIOR_H
#define RESETBEHAVIOR_H

#include "Task.h"

class ResetBehavior : public Task
{
    public:
        bool execute(int);
        ResetBehavior();
        virtual ~ResetBehavior();
};

#endif // ATTACKTASK_H