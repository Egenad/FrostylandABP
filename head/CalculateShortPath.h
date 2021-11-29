#ifndef CALCULATESHORTPATH_H
#define CALCULATESHORTPATH_H

#include "Task.h"

class CalculateShortPath: public Task
{
    public:
        bool execute(int);
        CalculateShortPath();
        virtual ~CalculateShortPath();
    private:
        bool calcule(int);
};

#endif // CALCULATESHORTPATH_H