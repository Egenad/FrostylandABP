#ifndef FOLLOWTASK_H
#define FOLLOWTASK_H

#include "Task.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Enemy.h"
#include "Door.h"

class FollowTask: public Task
{
    public:
        bool execute(int);
        FollowTask();
        virtual ~FollowTask();
    private:
        bool seek(int);
};

#endif // MOVETONODE_H